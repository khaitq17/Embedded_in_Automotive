## Bài 1: Thao tác với bộ nhớ Flash
Thực hiện các thao tác với bộ nhớ Flash: 
- Xóa 1 page trong bộ nhớ Flash.
- Ghi nhiều byte dữ liệu vào vùng nhớ trong Flash.
- Đọc nhiều byte dữ liệu từ vùng nhớ trong Flash và lưu vào bộ đệm.
- Ghi 1 byte dữ liệu vào vùng nhớ trong Flash.

## Bài 2: Viết chương trình Boot cho STM32F1
Nạp 1 chương trình Blink Led vào Flash của STM32F1 ở một vùng nhớ khác 0x08000000, sau đó viết chương trình Boot để điều khiển Vector Table trỏ đến vùng nhớ này để chạy chương trình.