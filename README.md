# payloadUAV

Firmware NuttX cho board điều khiển payload gắn trên UAV (STM32F411, dạng BlackPill).
Board nhận lệnh từ flight controller (FC) qua MAVLink, và (với payload `drop`) từ tay điều khiển qua SBUS, rồi xuất PWM ra servo/bơm/tời.

## 1. Bắt đầu nhanh

```bash
git clone --recurse-submodules https://github.com/HuyWnW2326/payloadUAV.git
cd payloadUAV
./build.sh drop        # build target "drop"
./flash.sh drop        # nạp qua ST-Link (SWD) bằng openocd
```

Cần cài sẵn: `arm-none-eabi-gcc`, `cmake`, `ninja`, `openocd`, và quyền truy cập ST-Link (udev rules).

Lưu ý:
- `build.sh` ghi `apps/external/CMakeLists.txt` vào submodule `apps` mỗi lần chạy, nên `git status` sẽ báo submodule `apps` bị thay đổi. Đây là bình thường.
- Chỉ configure lần đầu (khi chưa có `build/<target>/build.ninja`). Nếu sửa `defconfig` thì xóa `build/<target>` rồi build lại.
- Submodule: `nuttx` (apache/nuttx), `apps` (apache/nuttx-apps), `common/mavlink` (mavlink/c_library_v2).

## 2. Cấu trúc repo

| Thư mục | Nội dung |
|---|---|
| `boards/arm/stm32/stm32f411-minimum/` | Định nghĩa board: `include/board.h` (clock, chân), `src/` (khởi tạo), `configs/<target>/defconfig` |
| `common/mavlink_handler` | Nhận và xử lý MAVLink (UART + USB CDC), param, heartbeat |
| `common/param` | Lưu param trong flash sector 7 |
| `common/pwm` | Driver PWM (chế độ servo µs hoặc duty %) |
| `common/sbus` | Driver SBUS và lớp theo dõi kênh (`sbus_channel_task`) |
| `common/payload_arbiter` | Chọn nguồn lệnh (SBUS hoặc MAVLink) và thực thi 1 lần |
| `payloads/<target>` | Logic riêng từng payload (`*_main.c`) |

## 3. Các target

| Target | Chức năng | Ghi chú |
|---|---|---|
| `drop` | Thả hàng: servo 1-6 theo `DO_SET_ACTUATOR`; servo 6 là kênh kích thả (MAVLink `DO_SET_SERVO` số 6 hoặc công tắc SBUS) | MAVLink + SBUS + arbiter |
| `winch` | Tời: `DO_SET_ACTUATOR` param1 > 0 thì duty 100%, ngược lại 0% | Chỉ MAVLink |
| `agriculture` | Phun thuốc: bơm điều khiển theo tốc độ bay | Chỉ MAVLink |

Mỗi target có `defconfig` riêng; `INIT_ENTRYPOINT` trỏ đến `<target>_payload_main`, nên board tự chạy payload khi bật nguồn.

## 4. Phần cứng

### Cổng nối tiếp (theo `defconfig` và code)

| Thiết bị NuttX | UART | Chân | Baud | Dùng cho |
|---|---|---|---|---|
| `/dev/ttyS0` | USART2 | PA2 (TX) / PA3 (RX) | 115200 | Console NSH |
| `/dev/ttyS1` | USART1 | PA9 (TX) / PA10 (RX) | 57600 | MAVLink tới FC |
| `/dev/ttyS2` | USART6 | PC6 (TX) / PC7 (RX) | 100000, 8E2 | SBUS (chỉ target `drop`) |
| `/dev/ttyACM0` | USB CDC | USB OTG FS | - | MAVLink tới QGroundControl |

- SBUS chuẩn là tín hiệu đảo mức, STM32F411 không có chức năng đảo mức RX bằng phần cứng UART và code cũng không đảo bằng phần mềm, nên cần bộ đảo mức bên ngoài giữa receiver và chân PC7.
- Số thứ tự `ttyS*` phụ thuộc UART nào được bật và UART nào là console. Nếu bật/tắt UART trong `defconfig` thì phải kiểm tra lại `MAVLINK_UART_DEVICE` và `SBUS_UART_DEVICE` trong `payloads/*/*_main.c`.

### Đầu ra PWM

| Servo | Timer/Kênh | Chân | Thiết bị |
|---|---|---|---|
| 1 | TIM2 CH1 | PA0 | `/dev/pwm0` |
| 2 | TIM2 CH2 | PA1 | `/dev/pwm0` |
| 3 | TIM3 CH1 | PA6 | `/dev/pwm1` |
| 4 | TIM3 CH2 | PA7 | `/dev/pwm1` |
| 5 | TIM3 CH3 | PB0 | `/dev/pwm1` |
| 6 | TIM3 CH4 | PB1 | `/dev/pwm1` |

Trong `drop`, servo 6 là kênh kích thả (nghỉ 1000 µs, thả 2000 µs).

### Khác
- Xung nhịp: thạch anh 25 MHz, SYSCLK 96 MHz (`board.h`). Có sẵn dòng cấu hình cho thạch anh 8 MHz đang bị comment; nếu đổi thạch anh phải sửa cả `STM32_BOARD_XTAL` lẫn `PLLM`.
- LED trạng thái: PC13.
- Nạp firmware: ST-Link qua SWD (`flash.sh`, `adapter speed 100`, reset kiểu `srst_only`).

### Khi đổi phần cứng, sửa ở đâu

| Thay đổi | File |
|---|---|
| Thạch anh, clock | `boards/.../include/board.h` |
| Chân UART/PWM | `board.h` (định nghĩa `GPIO_*`) và `defconfig` (`CONFIG_STM32_*`) |
| Tên cổng MAVLink/SBUS, baud | `payloads/<target>/<target>_main.c` (`MAVLINK_UART_*`, `SBUS_UART_DEVICE`) và `defconfig` |
| Kênh SBUS, ngưỡng, debounce | Đầu file `payloads/drop/drop_main.c` (`SBUS_CH_*`, `SBUS_THRESHOLD`, `SBUS_DEBOUNCE_N`) |
| Servo dùng làm kênh thả, xung nghỉ/thả | `payloads/drop/drop_main.c` (`DROP_SERVO_*`) |
| Dải xung từng servo | Param `SERVOx_MIN/MAX` (sửa qua QGC, xem mục 6) |

## 5. Kiến trúc chạy (target `drop`)

Các task được tạo trong `drop_payload_main`:

| Task | Ưu tiên | Việc làm |
|---|---|---|
| `drop_executor` | 120 | Chờ semaphore từ arbiter, gọi `drop_execute_cb` để đặt xung thả |
| `mavlink_recv` | 100 | Đọc UART + USB, xử lý lệnh, gửi heartbeat 1 Hz |
| `sbus_recv` | 100 | Đọc SBUS, phát hiện cạnh và mức kênh, báo failsafe |

**Arbiter** có 3 chế độ:

| Chế độ | Khi nào | Nguồn được phép kích thả |
|---|---|---|
| `FAILSAFE_LOCKED` | Mặc định khi khởi động; khi mất tín hiệu SBUS | Không ai |
| `MANUAL_SBUS` | Kênh SBUS 5 > 1200 | Chỉ SBUS (kênh 6, sau debounce) |
| `AUTO_MAVLINK` | Kênh SBUS 5 <= 1200 | Chỉ MAVLink (`DO_SET_SERVO` servo 6) |

Kích thả chỉ thực thi **một lần** (cờ `executed`); muốn thả lại phải gọi `payload_arbiter_rearm()`, hiện chưa có chỗ nào trong code gọi hàm này nên sau khi thả cần khởi động lại board.

Vì mặc định là `FAILSAFE_LOCKED`, sau khi bật nguồn phải nhận được ít nhất một khung SBUS hợp lệ (để kênh mode đặt lại chế độ) thì mới thả được, kể cả ở chế độ MAVLink.

## 6. Tham số (param)

- Lưu trong flash sector 7 tại `0x08060000` (128 KB), ghi khi nhận `PARAM_SET` từ QGC. Nạp lại firmware không làm mất param, nhưng firmware không được to đến mức chạm vùng này.
- Danh sách và giá trị mặc định (`common/param/param.c`):

| Param | Mặc định | Ý nghĩa |
|---|---|---|
| `SERVO1_MIN` | 1500 | Xung tối thiểu servo 1 (µs) |
| `SERVO1_MAX` | 2000 | Xung tối đa servo 1 |
| `SERVO2..6_MIN` | 1000 | Xung tối thiểu servo 2 đến 6 |
| `SERVO2..6_MAX` | 2000 | Xung tối đa servo 2 đến 6 |
| `SERVO_NUM` | 6 | Số servo |
| `PWM_FREQ` | 50 | Tần số PWM (Hz) |
| `MAV_SYS_ID` | 1 | System ID MAVLink của board |
| `MAV_COMP_ID` | 236 | Component ID MAVLink của board |

- Thêm/xóa param: sửa bảng `g_params[]` trong `param.c` và enum `param_id_e` trong `param.h`, giữ đúng thứ tự.

## 7. Cách kiểm tra khi vận hành

- Mở console: nối USART2 (PA2/PA3) vào bộ chuyển USB-UART, `115200 8N1`, ví dụ `minicom -D /dev/ttyUSB0 -b 115200`.
- Log khởi động bình thường của `drop`:
  ```
  === Payload Controller ===
  ✓ PWM OK
  ✓ MAVLink OK
  ✓ Executor task started
  ✓ MAVLink task started
  ✓ SBUS task started
  [Payload] ---------------- All systems running ---------------
  ```
- Kiểm tra MAVLink: kết nối QGC qua USB (`/dev/ttyACM0`), xem danh sách param, gửi `DO_SET_ACTUATOR`; log in `[MAVLink] Actuator n -> xxxx us`.
- Kiểm tra SBUS: gạt kênh 5 và kênh 6 trên tay điều khiển, log in `[Drop] FIRE (source=SBUS)`.
- Target `agriculture` cần FC gửi `HEARTBEAT` và `LOCAL_POSITION_NED` qua cổng MAVLink.

## 8. Hạn chế hiện tại

- `winch`: `handle_do_set_actuator` gọi `pwm_driver_set_duty(..., 1, ...)` trong khi chỉ có một kênh (chỉ số 0) được thêm vào driver, nên lệnh trả về `MAV_RESULT_FAILED`. Sửa: đổi chỉ số 1 thành 0 trong `payloads/winch/winch_main.c`.
- `agriculture`: `density` và `spray_width` không được gán ở đâu (mặc định 0), nên ở chế độ Mission lưu lượng tính ra bằng 0 và bơm không bật; chế độ test (`param2` khác 0) vẫn hoạt động.
- Không có watchdog/failsafe cho MAVLink: khi mất liên kết với FC, đầu ra giữ nguyên giá trị lệnh cuối (chỉ SBUS có failsafe).
- Nhiều `printf` trong đường xử lý MAVLink; nếu console chậm có thể ảnh hưởng thời gian phản hồi.
- Ghi param xóa cả sector 7 khi đang chạy, CPU có thể đứng tạm thời; nên tránh ghi param khi đang bay.

## 9. Thêm một payload mới

1. Copy `payloads/drop` thành `payloads/<tên>`, đổi tên file `.c`, tên hàm `<tên>_payload_main` và `NAME <tên>_payload` trong `CMakeLists.txt`.
2. Tạo `boards/arm/stm32/stm32f411-minimum/configs/<tên>/defconfig` (copy từ `drop`), đổi `CONFIG_BASE_DEFCONFIG` và `CONFIG_INIT_ENTRYPOINT`.
3. Viết logic trong `mavlink_ops_t` (và SBUS/arbiter nếu cần).
4. `./build.sh <tên>` rồi `./flash.sh <tên>`.
