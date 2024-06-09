# Embedded-Automotive

## Bài 2: GPIO

### 1. Cấp xung clock cho GPIO
Module RCC (Reset and Clock Control) cung cấp các hàm để cấu hình xung clock cho ngoại vi qua các Bus tương ứng.
![new](Picture1.png)
```
RCC_APB1PeriphClockCmd

RCC_APB2PeriphClockCmd
	
RCC_AHBPeriphClockCmd
```
- Tham số đầu tiên là ngoại vi cần cấu hình clock.
- Tham số thứ 2 là giá trị quy định cấp (ENABLE) hay ngưng (DISABLE) xung clock cho ngoại vi đó.

### 2. Cấu hình GPIO
- Các tham số cho GPIO được tổ chức trong 1 struct **GPIO_InitTypeDef**:
	- GPIO_Pin: Chọn chân
 	- GPIO_Mode: Chọn chế độ
    - GPIO_Speed: Chọn tốc độ
- Dùng hàm **GPIO_Init** để khởi tạo GPIO:
    - GPIO_TypeDef: GPIO cần cấu hình
    - &GPIO_InitStruct: Con trỏ trỏ tới biến TypeDef vừa được khởi tạo

### 3. Một số hàm thông dụng trong GPIO
```
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
  
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
	
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
```

## BÀI 3: Ngắt và Timer
### 1. Ngắt
- Ngắt là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển. Khi xảy ra ngắt, MCU phải dừng chương trình chính và thực thi chương trình ngắt.
- Program Counter(PC): Trong hàm main, khi đang thực hiện 1 lệnh, PC sẽ trỏ tới lệnh tiếp theo.
- Các loại ngắt thông dụng:
    - **Reset**
    - **Ngắt ngoài**: Xảy ra khi có thay đổi điện áp trên các chân GPIO đưuọc cấu hình làm ngõ vào ngắt.
    - **Ngắt Timer**: Xảy ra khi giá trị trong thanh ghi đếm của Timer tràn.
    - **Ngắt truyền thông**: Xảy ra khi có sự truyền/nhận dữ liệu giữa các MCU hay giữa MCU với các thiết bị bên ngoài.
- Độ ưu tiên ngắt
    - Độ ưu tiên là khác nhau ở các ngắt. Nó xác định ngắt nào đưuọc quyền thực thi khi nhiều ngắt xảy ra đồng thời.
    - Ngắt nào có số thứ tự ưu tiên càng thấp thì có quyền càng cao.

### 2. Timer
- Timer là 1 mạch digital logic có vai trò đếm mỗi chu kỳ clock (đếm lên hoặc đếm xuống).
Cấu hình Timer:    
- Cấp xung cho Timer
- Cấu hình cho Timer trong struct **TIM_TimeBaseInitTypeDef**:
    - TIM_ClockDivision: Chia tần số
 	  - TIM_Prescaler: Quy định sau bao nhiêu dao động thì đếm lên 1 lần
  	- TIM_Period: Quy định thời gian 1 chu kỳ
   	- TIM_Mode: Chọn chế độ (Đếm lên hoặc đếm xuống)

## BÀI 4: Các chuẩn giao tiếp cơ bản
### 1. SPI - Serial Peripheral Interface
- Là chuẩn giao tiếp đồng bộ.
- Hoạt động ở chế độ song công (Có thể truyền - nhận cùng thời điểm).
- Các đầu nối:
    - **SCK** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
    - **MISO** (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
    - **MOSI** (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.
    - **SS** (Slave Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).
[new](https://imgur.com/hBls0FV)
- Quá trình truyền nhận dữ liệu:
    - Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận.
    - Master cấp xung clock, với mỗi xung clock, 1 bit sẽ được truyền từ Master đến Slave và ngược lại.
    - Các thanh ghi cập nhật giá trị và dịch 1 bit.
    - Lặp lại quá trình đến khi truyền xong 8 bit thanh ghi.
[new](https://imgur.com/iT4BTRS)
- Các chế độ hoạt động:
    - Có 4 chế độ hoạt động phụ thuộc **Clock Polarity** (CPOL) và **Clock Phase** (CPHA).
    - CPOL: 
        - CPOL = 0: Xung clock ban đầu ở mức 0.
        - CPOL = 1: Xung clock ban đầu ở mức 1.
    - CPHA:
        - CPHA = 0: Truyền bit trước rồi mới cấp xung.
        - CPHA = 1: Cấp xung trước rồi mới truyền bit.
| SPI Mode | CPOL | CPHA |
| :--- | :--- | :--- | 
| 1 | 0 | 0 |
| 2 | 0 | 1 |
| 3 | 1 | 0 | 
| 4 | 1 | 1 | 

### 2. I2C - Inter-Integrated Circuit
- Là chuẩn giao tiếp đồng bộ.
- Hoạt động ở chế độ bán song công.
- Một Master giao tiếp được với nhiều Slave.
- Các đầu nối:
    - **SDA** (Serial Data): Đường truyền cho Master và Slave để gửi và nhận dữ liệu.
    - **SCL** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
[new](https://imgur.com/6JN0vw1)
- Quá trình truyền nhận dữ liệu:
    - Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
    - Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.
    - Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.
    - ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.
    - Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
    - Tương tự cũng có ACK để chờ phản hồi.
	- Stop: Điều kiện: Chân SDA lên mức 1 trước chân SCL.
[new](https://imgur.com/1FE9SPj)

### 3. UART - Universal Asynchronous Receiver - Transmitter
- Là chuẩn giao tiếp **KHÔNG** đồng bộ.
- Hoạt động ở chế độ song công.
- Dùng Timer nội để phân biệt 2 bit liền kề.
- Tốc độ truyền: Baudrate = Số bit truyền/1s
- Các đầu nối:
    - TX: Truyền.
    - RX: Nhận.
[new](https://imgur.com/BLGyJE4)
- Quá trình truyền nhận dữ liệu:
    - Start: 1 bit.
    - Bit dữ liệu: 5 đến 9 bit.
    - Bit chẵn lẻ:
        - Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
        - Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
    - Stop: 1 đến 2 bit.
[new](https://imgur.com/f1kihwM)

=======
# BÀI 2: GPIO

## Cấp xung clock cho GPIO
<p>Module RCC cung cấp các hàm để cấu hình xung clock cho ngoại vi qua các Bus tương ứng.</p>

	RCC_APB1PeriphClockCmd

 	RCC_APB2PeriphClockCmd
	
 	RCC_AHBPeriphClockCmd
<p>Tham số đầu tiên là ngoại vi cần cấu hình clock.</p>
<p>Tham số thứ 2 là giá trị quy định cấp (ENABLE) hay ngưng (DISABLE) xung clock cho ngoại vi đó.</p>

## Cấu hình GPIO
<p>Các tham số cho GPIO được tổ chức trong 1 struct <strong>GPIO_InitTypeDef</strong>:</p>

	GPIO_Pin: Chọn chân

 	GPIO_Mode: Chọn chế độ

  	GPIO_Speed: Chọn tốc độ
<p>Dùng hàm <strong>GPIO_Init</strong> để khởi tạo GPIO:</p>

	GPIO_TypeDer: GPIO cần cấu hình

 	&GPIO_InitStruct: Con trỏ trỏ tới biến TypeDef vừa được khởi tạo
## Một số hàm thông dụng trong GPIO

 	uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
  
	uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

	uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

	uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

	void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//0x0000….0010

	void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

	void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
	
 	void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);


# BÀI 3: Ngắt và Timer

## Ngắt
<p>Ngắt là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển. Khi xảy ra ngắt, MCU phải dừng chương trình chính và thực thi chương trình ngắt.</p>
<p>Program Counter(PC): Trong hàm main, khi đang thực hiện 1 lệnh, PC sẽ trỏ tới lệnh tiếp theo.</p>
<h3> Các loại ngắt thông dụng</h3>
<p><strong>Reset</strong></p>
<p><strong>Ngắt ngoài</strong>: Xảy ra khi có thay đổi điện áp trên các chân GPIO đưuọc cấu hình làm ngõ vào ngắt.</p>
<p><strong>Ngắt Timer</strong>: Xảy ra khi giá trị trong thanh ghi đếm của Timer tràn.</p>
<p><strong>Ngắt truyền thông</strong>: Xảy ra khi có sự truyền/nhận dữ liệu giữa các MCU hay giữa MCU với các thiết bị bên ngoài.</p>

<h3> Độ ưu tiên ngắt</h3>
<p>Độ ưu tiên là khác nhau ở các ngắt. Nó xác định ngắt nào đưuọc quyền thực thi khi nhiều ngắt xảy ra đồng thời.</p>
<p>Ngắt nào có số thứ tự ưu tiên càng thấp thì có quyền càng cao.</p>

## Timer
<p>Timer là 1 mạch digital logic có vai trò đếm mỗi chu kỳ clock (đếm lên hoặc đếm xuống).</p>
<h3> Cấu hình Timer</h3>
<p>Cấp xung clock cho Timer.</p>
<p>Cấu hình cho Timer trong struct <strong>TIM_TimeBaseInitTypeDef</strong>:</p>

	TIM_ClockDivision: Chia tần số
 	TIM_Prescaler: Quy định sau bao nhiêu dao động thì đếm lên 1 lần
  	TIM_Period: Quy định thời gian 1 chu kỳ
   	TIM_Mode: Chọn chế độ

# Bài 4: Các chuẩn giao tiếp cơ bản

## SPI - Serial Peripheral Interface
<p>Là chuẩn giao tiếp đồng bộ.</p>
<p>Hoạt động ở chế độ song công (Có thể truyền - nhận cùng thời điểm).</p>

<h3>Các đầu nối</h3>
<ul>
  <li>SCK (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.</li>
  <li>MISO (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.</li>
  <li>MOSI (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave. </li>
  <li>SS (Slave Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).</li>
</ul>

<h3>Quá trình truyền nhận dữ liệu</h3>
<ul>
  <li>Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận.</li>
  <li>Master cấp xung clock, với mỗi xung clock, 1 bit sẽ được truyền từ Master đến Slave và ngược lại.</li>
  <li>Các thanh ghi cập nhật giá trị và dịch 1 bit.</li>
  <li>Lặp lại quá trình đến khi truyền xong 8 bit thanh ghi.</li>
</ul>

<h3>Các chế độ hoạt động</h3>
<p>Có 4 chế độ hoạt động phụ thuộc Clock Polarity (CPOL) và Phase (CPHA).</p>
<p>CPOL:</p>
<ul>
  <li>CPOL = 0: Xung clock ban đầu ở mức 0.</li>
  <li>CPOL = 1: Xung clock ban đầu ở mức 1.</li>
</ul>
<p>CPHA:</p>
<ul>
  <li>CPHA = 0: Truyền bit trước rồi mới cấp xung.</li>
  <li>CPHA = 1: Cấp xung rồi mới truyền bit.</li>
</ul>

## I2C - Inter-Integrated Circuit
<p>Là chuẩn giao tiếp đồng bộ.</p>
<p>Hoạt động ở chế độ bán song công.</p>
<p>Một Master giao tiếp được với nhiều Slave.</p>

<h3>Các đầu nối</h3>
<ul>
  <li>SDA (Serial Data): Đường truyền cho Master và Slave để gửi và nhận dữ liệu.</li>
  <li>SCL (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.</li>
</ul>

<h3>Quá trình truyền nhận dữ liệu</h3>
<ul>
	<li>Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.</li>
  <li>Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.</li>
  <li>Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.</li>
  <li>ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.</li>
  <li>Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.</li>
  <li>Tương tự cũng có ACK để chờ phản hồi.</li>
	<li>Stop: Điều kiện: Chân SDA lên mức 1 trước chân SCL.</li>
</ul>

## UART - Universal Asynchronous Receiver - Transmitter
<p>Là chuẩn giao tiếp <strong>KHÔNG</strong> đồng bộ.</p>
<p>Hoạt động ở chế độ song công.</p>
<p>Dùng Timer nội để phân biệt 2 bit liền kề.</p>
<p>Tốc độ truyền: Baudrate = Số bit truyền/1s</p>

<h3>Các đầu nối</h3>
<p>Gồm 2 đường truyền dứ liệu độc lập và không cần chân Clock.</p>
<ul>
  <li>TX: Truyền.</li>
  <li>RX: Nhận.</li>
</ul>

<h3>Quá trình truyền nhận dữ liệu</h3>
<ul>
  <li>Start: 1 bit.</li>
  <li>Bit dữ liệu: 5 đến 9 bit.</li>
  <li>Bit chẵn lẻ:
		<ul>
      <li>Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.</li>
      <li>Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.</li>
    </ul>
	</li>
  <li>Stop: 1 đến 2 bit.</li>
</ul>