## Bài 1: Ngắt ngoài
Sử dụng nút nhấn để tạo ra ngắt ngoài, mỗi lần nhấn nút tăng biến đếm `count` lên 1.

## Bài 2: Ngắt Timer
Cấu hình Timer để tạo ra ngắt Timer. Khi bộ đếm Timer tràn tăng biến đếm `count` lên 1. 

Viết hàm `delay` phụ thuộc vào biến `count` để tạo ra độ trễ và chớp tắt LED 1s cho chân PC13.

## Bài 3: Ngắt truyền thông
Dùng giao thức UART để gửi dữ liệu qua ngắt. 

**Lưu ý:** Trong các hàm xử lý ngắt `IRQHandler`, kiểm tra cờ ngắt có bật không sau đó mới xử lý ngắt và sau khi xử lý xong phải xóa cờ ngắt.