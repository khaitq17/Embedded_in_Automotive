## Sử dụng DMA để tăng tốc độ truyền dữ liệu
Giao tiếp SPI giữa 2 vi điều khiển STM32F1. Trong đó:
- STM32F1 Master:
    - Dùng ADC đọc giá trị biến trở (12 bit) và chuyển đổi giá trị về góc (0° đến 180°).
    - Sau đó, truyền dữ liệu góc cho Slave qua giao thức SPI.
- STM32F1 Slave:
    - Nhận dữ liệu truyền từ Master, sử dụng DMA.
    - Từ giá trị góc nhận được, thông qua PWM để quay góc Servo tương ứng.