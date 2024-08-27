<details>
	<summary><strong>BÀI 1: GPIO</strong></summary>
	
# Bài 1: GPIO

## 1.1 Cấp xung clock cho GPIO
Module RCC (Reset and Clock Control) cung cấp các hàm để cấu hình xung clock cho ngoại vi qua các Bus tương ứng.
![Picture1](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/daacd745-60e6-4499-bb41-86c84afb9edd)
```
RCC_APB1PeriphClockCmd

RCC_APB2PeriphClockCmd
	
RCC_AHBPeriphClockCmd
```
- Tham số đầu tiên là ngoại vi cần cấu hình clock.
- Tham số thứ 2 là giá trị quy định cấp (ENABLE) hay ngưng (DISABLE) xung clock cho ngoại vi đó.

## 1.2 Cấu hình GPIO
- Các tham số cho GPIO được tổ chức trong 1 struct **GPIO_InitTypeDef**:
	- `GPIO_Pin`: Chọn chân
 	- `GPIO_Mode`: Chọn chế độ
    - `GPIO_Speed`: Chọn tốc độ
- Dùng hàm **GPIO_Init** để khởi tạo GPIO:
    - `GPIO_TypeDef`: GPIO cần cấu hình
    - `&GPIO_InitStruct`: Con trỏ trỏ tới biến TypeDef vừa được khởi tạo

## 1.3 Một số hàm thông dụng trong GPIO
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
</details>

<details>
	<summary><strong>BÀI 2: NGẮT VÀ TIMER</strong></summary>

 # BÀI 2: NGẮT VÀ TIMER
## 2.1 Lý thuyết ngắt
- **Ngắt** là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển. Khi xảy ra ngắt, MCU phải dừng chương trình chính và thực thi chương trình ngắt.
- **Program Counter(PC)**: Trong hàm main, khi đang thực hiện 1 lệnh, PC sẽ trỏ tới lệnh tiếp theo.
- Các loại ngắt thông dụng:
    - **Reset**
    - **Ngắt ngoài**: Xảy ra khi có thay đổi điện áp trên các chân GPIO đưuọc cấu hình làm ngõ vào ngắt.
    - **Ngắt Timer**: Xảy ra khi giá trị trong thanh ghi đếm của Timer tràn.
    - **Ngắt truyền thông**: Xảy ra khi có sự truyền/nhận dữ liệu giữa các MCU hay giữa MCU với các thiết bị bên ngoài.
- Độ ưu tiên ngắt
    - Độ ưu tiên là khác nhau ở các ngắt. Nó xác định ngắt nào được quyền thực thi khi nhiều ngắt xảy ra đồng thời.
    - Ngắt nào có số thứ tự ưu tiên càng thấp thì có quyền càng cao.

## 2.2 Timer
**Timer** là 1 mạch digital logic có vai trò đếm mỗi chu kỳ clock (đếm lên hoặc đếm xuống).

Cấu hình Timer:    
- Cấp xung cho Timer
- Cấu hình cho Timer trong struct **TIM_TimeBaseInitTypeDef**:
    - `TIM_ClockDivision`: Chia tần số
 	- `TIM_Prescaler`: Quy định sau bao nhiêu dao động thì đếm lên 1 lần
  	- `TIM_Period`: Quy định thời gian 1 chu kỳ
   	- `TIM_Mode`: Chọn chế độ (Đếm lên hoặc đếm xuống)
</details>
	
<details>
	<summary><strong>BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN</strong></summary>

 # BÀI 3: CÁC CHUẨN GIAO TIẾP CƠ BẢN
## 3.1 SPI - Serial Peripheral Interface
- Là chuẩn giao tiếp đồng bộ.
- Hoạt động ở chế độ song công (Có thể truyền - nhận cùng thời điểm).
### 3.1.1 Các đầu nối
- **SCK** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- **MISO** (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- **MOSI** (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.
- **SS** (Slave Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).
  
![Picture2](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/59a2f364-6d36-4043-927f-1324d5c19944)
### 3.1.2 Quá trình truyền nhận dữ liệu
- Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận.
- Master cấp xung clock, với mỗi xung clock, 1 bit sẽ được truyền từ Master đến Slave và ngược lại.
- Các thanh ghi cập nhật giá trị và dịch 1 bit.
- Lặp lại quá trình đến khi truyền xong 8 bit thanh ghi.
  
![Picture3](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/f984fc6c-1364-4c0d-8566-e4bdb93302db)
### 3.1.3 Các chế độ hoạt động
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

## 3.2 I2C - Inter-Integrated Circuit
- Là chuẩn giao tiếp đồng bộ.
- Hoạt động ở chế độ bán song công.
- Một Master giao tiếp được với nhiều Slave.
### 3.2.1 Các đầu nối
- **SDA** (Serial Data): Đường truyền cho Master và Slave để gửi và nhận dữ liệu.
- **SCL** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
      
![Picture4](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/89a016e7-34fa-443d-8736-0102a2a6a62a)
### 3.2.2 Quá trình truyền nhận dữ liệu
- Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
- Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.
- Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.
- ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.
- Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
- Tương tự cũng có ACK để chờ phản hồi.
- Stop: Điều kiện: Chân SDA lên mức 1 trước chân SCL.
   
 ![Picture5](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/d5ba043a-1d63-4de2-a7a6-bdb783e4acee)

## 3.3 UART - Universal Asynchronous Receiver - Transmitter
- Là chuẩn giao tiếp **KHÔNG** đồng bộ.
- Hoạt động ở chế độ song công.
- Dùng Timer nội để phân biệt 2 bit liền kề.
- Tốc độ truyền: Baudrate = Số bit truyền/1s
### 3.3.1 Các đầu nối
- TX: Truyền.
- RX: Nhận.
      
![Picture6](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9a52fd6e-eb76-43f4-aa81-9cce344ae3a7)
### 3.3.2 Quá trình truyền nhận dữ liệu
- Start: 1 bit.
- Bit dữ liệu: 5 đến 9 bit.
- Bit chẵn lẻ:
	- Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
	- Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
- Stop: 1 đến 2 bit.
      
![Picture7](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/56943f89-3f0a-4785-b944-51966027ff2b)
</details>

<details>
	<summary><strong>BÀI 4: GIAO TIẾP SPI</strong></summary>

# BÀI 4: GIAO TIẾP SPI
## 4.1 SPI Software
**SPI Software** là cách “mô phỏng” bằng việc tạo ra một giao thức truyền thông giống SPI nhưng chỉ sử dụng GPIO của vi điều khiển.
### 4.1.1 Cấu hình GPIO cho SPI Software
SPI dùng 4 chân để truyền nhận, gồm MISO, MOSI, CS và SCK.

![Picture8](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/35ea2e93-5df0-4663-b101-b708694557df)
- **SCK** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- **MISO** (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- **MOSI** (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.
- **CS** (Chip Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường CS tương ứng xuống mức 0 (Low).

Định nghĩa 4 chân sử dụng SPI:
```
#define SPI_SCK_Pin GPIO_Pin_0
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA
```
Cấu hình GPIO:
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin| SPI_MOSI_Pin| SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}
```
Tạo xung Clock:
```
void Clock(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);		// Kéo chân SCK lên 1	
	delay_ms(4);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);	// Kéo chân SCK xuống 0
	delay_ms(4);
}
```
### 4.1.2 Khởi tạo các chân cho SPI
```
void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}
```
### 4.1.3 Hàm truyền
Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu.
- Kéo CS xuống 0.
	- Truyền 1 bit.
	- Dịch 1 bit.
	- Gửi Clock
- Kéo CS lên 1.
```
void SPI_Master_Transmit(uint8_t u8Data)
{
	uint8_t u8Mask = 0x80;// 0b10000000
	uint8_t tempData;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);		// Kéo CS xuống 0
	delay_ms(1);
	for(int i=0; i<8; i++){			// Truyền lần lượt 8 bit
		tempData = u8Data & u8Mask;
		if(tempData){
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		} else{
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		u8Data=u8Data<<1;
		Clock();		// Gửi Clock
	}
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);		// Kéo CS lên 1
	delay_ms(1);
}
```
### 4.1.4 Hàm nhận
- Kiểm tra CS == 0?.
	- Kiểm tra Clock == 1?
	- Đọc data trên MOSI, ghi vào biến.
	- Dịch 1 bit.
- Kiểm tra CS == 1?
```
uint8_t SPI_Slave_Receive(void){
	uint8_t u8Mask = 0x80;
	uint8_t dataReceive =0x00;//0b11000000
	uint8_t temp = 0x00, i=0;
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));		
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));		// Kiểm tra CS == 0
	for(i=0; i<8;i++)
    	{ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
		while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)) 
			temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);	// Đọc data trên MOSI
		dataReceive=dataReceive<<1;		// Dịch 1 bit
		dataReceive=dataReceive|temp;	// Ghi vào biến
    		}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));		// Kiểm tra CS == 1
	}
	return dataReceive;
}
```

## 4.2 SPI Hardware
Trên mỗi dòng vi điều khiển khác nhau module SPI sẽ được tích hợp, điều khiển bởi các thanh ghi, phần cứng, IO khác nhau gọi là SPI cứng (SPI Hardware). STM32F1 có 2 khối SPI: SPI1 ở APB2 và SPI2 ở APB1.
### 4.2.1 Cấu hình GPIO cho SPI Hardware
STM32 đã cấu hình sẵn các chân dành cho chức năng SPI. Khi sử dụng SPI1, ta định nghĩa các chân đã được thiết lập sẵn:
```
#define SPI1_NSS 	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO 	GPIO_Pin_6
#define SPI1_MOSI 	GPIO_Pin_7
#define SPI1_GPIO 	GPIOA
```
Cấu hình GPIO:
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS| SPI1_SCK| SPI1_MISO| SPI1_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}
```
### 4.2.2 Cấu hình SPI
Tương tự các ngoại vi khác, các tham số SPI được cấu hình trong Struct **SPI_InitTypeDef**:
- `SPI_Mode`: Quy định chế độ hoạt động của thiết bị SPI. 
- `SPI_Direction`: Quy định kiểu truyền của thiết bị.
- `SPI_BaudRatePrescaler`: Hệ số chia clock cấp cho Module SPI.
- `SPI_CPOL`: Cấu hình cực tính của SCK . Có 2 chế độ:
	- `SPI_CPOL_Low`: Cực tính mức 0 khi SCK không truyền xung.
	- `SPI_CPOL_High`: Cực tính mức 1 khi SCK không truyền xung.
- `SPI_CPHA`: Cấu hình chế độ hoạt động của SCK. Có 2 chế độ:
	- `SPI_CPHA_1Edge`: Tín hiệu truyền đi ở cạnh xung đầu tiên.
	- `SPI_CPHA_2Edge`: Tín hiệu truyền đi ở cạnh xung thứ hai.
- `SPI_DataSize`: Cấu hình số bit truyền. 8 hoặc 16 bit.
- `SPI_FirstBit`: Cấu hình chiều truyền của các bit là MSB hay LSB.
- `SPI_CRCPolynomial`: Cấu hình số bit CheckSum cho SPI.
- `SPI_NSS`: Cấu hình chân SS là điều khiển bằng thiết bị hay phần mềm.

```
void SPI_Config(){
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//72Mhs/16
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;//0b001001001
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}
```
### 4.2.3 Các hàm thông dụng
- Hàm `SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data)`, tùy vào cấu hình datasize là 8 hay 16 bit sẽ truyền đi 8 hoặc 16 bit dữ liệu. Hàm nhận 2 tham số là bộ SPI sử dụng và data cần truyền.
- Hàm `SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)` trả về giá trị đọc được trên SPIx. Hàm trả về 8 hoặc 16 bit data.
- Hàm `SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)` trả về giá trị 1 cờ trong thanh ghi của SPI. Các cờ thường được dùng:
	- `SPI_I2S_FLAG_TXE`: Cờ báo truyền, cờ này sẽ set lên 1 khi truyền xong data trong buffer.
	- `SPI_I2S_FLAG_RXNE`: Cờ báo nhận, cờ này set lên 1 khi nhận xong data.
	- `SPI_I2S_FLAG_BSY`: Cờ báo bận,set lên 1 khi SPI đang bận truyền nhận.

### 4.2.4 Hàm truyền
```
void SPI_Send1Byte(uint8_t data){
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);
   
    SPI_I2S_SendData(SPI1, data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0);
   
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);
}
```
### 4.2.5 Hàm nhận
```
uint8_t SPI_Receive1Byte(void){
    uint8_t temp;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==1);
    temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==0);
    return temp;
}
```
</details>

<details>
	<summary><strong>BÀI 5: GIAO TIẾP I2C</strong></summary>

# BÀI 5: GIAO TIẾP I2C
## 5.1 I2C Software
### 5.1.1 Cấu hình GPIO cho I2C Software
Định nghĩa các chân I2C:
I2C dùng 2 chân để truyền nhận, SCL và SDA.

![Picture4](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/89a016e7-34fa-443d-8736-0102a2a6a62a)
```
#define I2C_SCL 	GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7
#define I2C_GPIO 	GPIOB
```
Cấu hình GPIO:
```
void GPIO_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_SDA| I2C_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}
```
### 5.1.2 Cấu hình I2C
```
#define WRITE_SDA_0 	GPIO_ResetBits(I2C_GPIO, I2C_SDA)			// Kéo chân SDA xuống 0
#define WRITE_SDA_1 	GPIO_SetBits(I2C_GPIO, I2C_SDA)				// Kéo chân SDA lên 1
#define WRITE_SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)			// Kéo chân SCL xuống 0
#define WRITE_SCL_1 	GPIO_SetBits(I2C_GPIO, I2C_SCL)				// Kéo chân SDA lên 1
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)	// Đọc chân SDA
```
Khởi tạo I2C:
```
void I2C_Config(){
	WRITE_SDA_1;
	delay_us(1);
	WRITE_SCL_1;
	delay_us(1);
}
```
Hàm Start:
```
void I2C_Start(){	
	WRITE_SCL_1;  	
	delay_us(3);	
	WRITE_SDA_1;
	delay_us(3);
	WRITE_SDA_0;	// Điều kiện: Chân SDA xuống mức 0 trước chân SCL
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}
```
Hàm Stop:
```
void I2C_Stop(){
	
	WRITE_SDA_0;
	delay_us(3);
	WRITE_SCL_1; 	// Điều kiện: Chân SDA lên mức 1 trước chân SCL
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}
```
### 5.1.3 Hàm truyền
- Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu
	- Truyền 1 bit
	- Tạo Clock
	- Dịch 1 bit
- Chờ nhận lại ACK ở xung thứ 9
```
status I2C_Write(uint8_t u8Data){	
	uint8_t i;
	status stRet;
	for(int i=0; i< 8; i++){		
		if (u8Data & 0x80) {
			WRITE_SDA_1;
		} else {
			WRITE_SDA_0;
		}
		delay_us(3);
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(2);
		u8Data <<= 1;
	}
	WRITE_SDA_1;					
	delay_us(3);
	WRITE_SCL_1;					
	delay_us(3);
	
	if (READ_SDA_VAL) {	
		stRet = NOT_OK;				
	} else {
		stRet = OK;					
	}

	delay_us(2);
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}
```
### 5.1.4 Hàm nhận
- Hàm nhận sẽ nhận lần lượt 8 bit trong byte dữ liệu
- Kéo SDA lên 1
	- Đọc data trên SDA, ghi vào biến
	- Dịch 1 bit
- Gửi lại 1 tín hiệu ACK ở xung thứ 9
```
uint8_t I2C_Read(ACK_Bit _ACK){	
	uint8_t i;						
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);	
	for (i = 0; i < 8; ++i) {
		u8Ret <<= 1;
		WRITE_SCL_1;
		delay_us(3);
		if (READ_SDA_VAL) {
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}
	if (_ACK) {	
		WRITE_SDA_0;
	} else {
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}
```

## 5.2 I2C Hardware
STM32F1 có 2 khối I2C: I2C1 và I2C2 ở APB1.
### 5.2.1 Cấu hình GPIO cho I2C Hardware
STM32 đã cấu hình sẵn các chân dành cho chức năng I2C. Khi sử dụng I2C1, ta định nghĩa các chân đã được thiết lập sẵn:
```
#define I2C1_SCL 	GPIO_Pin_6
#define I2C1_SDA	GPIO_Pin_7
#define I2C1_GPIO 	GPIOB
```
Cấu hình GPIO:
```
void GPIO_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C1_SCL | I2C1_SDA; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
```
### 5.2.2 Cấu hình I2C
Tương tự các ngoại vi khác, các tham số I2C được cấu hình trong Struct **I2C_InitTypeDef**:
- `I2C_Mode`: Cấu hình chế độ hoạt động cho I2C:
	- `I2C_Mode_I2C`: Chế độ I2C FM(Fast Mode);
	- `I2C_Mode_SMBusDevice&I2C_Mode_SMBusHost`: Chế độ SM (Slow Mode).
- `I2C_ClockSpeed`: Cấu hình clock cho I2C, tối đa 100khz với SM và 400khz ở FM.
- `I2C_DutyCycle`: Cấu hình chu kì nhiệm vụ của xung:
	- `I2C_DutyCycle_2`: Thời gian xung thấp/xung cao = 2;
	- `I2C_DutyCycle_16_9`: Thời gian xung thấp/xung cao = 16/9;
- `I2C_OwnAddress1`: Cấu hình địa chỉ Slave.
- `I2C_Ack*`: Cấu hình ACK, có sử dụng ACK hay không.
- `I2C_AcknowledgedAddress`: Cấu hình số bit địa chỉ (7 hoặc 10 bit)
```
void I2C_Config(){
	I2C_InitTypeDef I2C_InitStructure;
	//Set the clock speed of I2C. It has to be equal with the external device
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	//I2C mode
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//I2C device adress
	I2C_InitStructure.I2C_OwnAddress1 = 0x33; 
	//I2C Acknowladge configuration
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//Enable the I2C with the prepared configuration
	I2C_Init(I2C1, &I2C_InitStructure);
	//And start the I2C 
	I2C_Cmd(I2C1, ENABLE);
}
```
### 5.2.3 Các hàm thông dụng
- Hàm `I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction)` gửi đi 7 bit address để xác định slave cần giao tiếp. Hướng truyền được xác định bởi I2C_Direction để thêm bit RW.
- Hàm `I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data)` gửi đi 8 bit data.
- Hàm `I2C_ReceiveData(I2C_TypeDef* I2Cx)` trả về 8 bit data.
- Hàm `I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)` trả về kết quả kiểm tra I2C_EVENT tương ứng:
	- `I2C_EVENT_MASTER_MODE_SELECT`: Đợi Bus I2C về chế độ rảnh.
	- `I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED`: Đợi xác nhận của Slave với yêu cầu ghi của Master.
	- `I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED`: Đợi xác nhận của Slave với yêu cầu đọc của Master.
	- `I2C_EVENT_MASTER_BYTE_TRANSMITTED`: Đợi truyền xong 1 byte data từ Master.
	- `I2C_EVENT_MASTER_BYTE_RECEIVED`: Đợi Master nhận đủ 1 byte data.
### 5.2.4 Hàm truyền
- Bắt đầu truyền nhận, bộ I2C sẽ tạo 1 tín hiệu start. Đợi tín hiệu báo Bus sẵn sàng.
- Gửi 7 bit địa chỉ để xác định slave. Đợi Slave xác nhận.
- Gửi/đọc các byte data, Đợi truyền xong.
- Sau đó kết thúc bằng tín hiệu stop.

![Picture9](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/0cad7138-b611-4cc8-9626-5edac43008d3)
```
void Send_I2C_Data(uint8_t data)
{
	I2C_SendData(I2C1, data);
	// wait for the data trasnmitted flag
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
```
### 5.2.5 Hàm nhận
```
uint8_t Read_I2C_Data()
{
	uint8_t data = I2C_ReceiveData(I2C1);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return data;
}
```
</details>

<details>
	<summary><strong>BÀI 6: GIAO TIẾP UART</strong></summary>

# BÀI 6: GIAO TIẾP UART
## 6.1 UART Software
### 6.1.1 Cấu hình GPIO cho UART Software

![Picture10](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/020f6237-8485-407a-aad9-e687fd17181f)
Định nghĩa các chân UART:
```
#define TX_Pin 		GPIO_Pin_9
#define RX_Pin 		GPIO_Pin_10
#define UART_GPIO 	GPIOA
```
Cấu hình GPIO:
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT_PP;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
}
```
### 6.1.2 Baudrate
Tốc độ baudrate được xác định bởi thời gian truyền đi 1 bit. Ta dùng tốc độ phổ thông 9600, ứng với mỗi bit là 105us.
Baaurate = 9600bits/s >> 0.10467ms for 1 bit = 104,67 us
=>> time delay ~~105 us
```
#define BRateTime 105
```
### 6.1.3 Cấu hình UART
```
void UART_Config(){
	GPIO_SetBits(UART_GPIO, TX_Pin);
	delay_us(1);
}
```
### 6.1.4 Hàm truyền
- Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu, sau khi tín hiệu start được gửi đi.
- Tạo start, delay 1 period time.
	- Truyền bit dữ liệu. mỗi bit truyền trong 1 period time.
	- Dịch 1 bit.
- Tạo stop, delay tương ứng với số bit stop.

![Picture11](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/3c402c9a-1025-4a59-a9ee-7955a6d16b61)
```
void UART_Transmit(const char DataValue)
{
	GPIO_WriteBit(UART_GPIO, TX_Pin, Bit_RESET);
	delay_us(BRateTime);
	for ( unsigned char i = 0; i < 8; i++ ){
		if( ((DataValue>>i)&0x1) == 0x1 ){
			GPIO_WriteBit(UART_GPIO, RX_Pin, Bit_SET);
		} else{
			GPIO_WriteBit(UART_GPIO, RX_Pin, Bit_RESET);
		}
	delay_us(BRateTime);
	}
	// Send Stop Bit
	GPIO_WriteBit(UART_GPIO, TX_Pin, Bit_SET);
	delay_us(BRateTime);
}
```
### 6.1.5 Hàm nhận
- Hàm nhận sẽ nhận lần lượt 8 bit 
- Chờ tín hiệu start từ thiết bị gửi.
- Delay 1,5 period time.
	- Đọc data trên RX, ghi vào biến.
	- Dịch 1 bit.
	- Delay 1 period time.
- Delay 1 period time và đợi stop bit.

![Picture12](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/2d3df3fe-402b-4560-a8e0-b61dcc6b2bf0)
```
unsigned char UART_Receive(void){
	unsigned char DataValue = 0;
	while(GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1);
	delay_us(BRateTime);
	delay_us(BRateTime/2);
	for ( unsigned char i = 0; i < 8; i++ ){
		if ( GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1 ){
			DataValue += (1<<i);}
		delay_us(BRateTime);
		}
		if ( GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1 ){
			delay_us(BRateTime/2);
			return DataValue;
		} 
}
```
### 6.1.6 Parity
Bit chẵn/lẻ được thêm vào cuối data.
```
typedef enum{
	Parity_Mode_NONE,
	Parity_Mode_ODD,
	Parity_Mode_EVENT
}Parity_Mode;
```
Tùy vào cấu hình parity là chẵn hay lẻ mà thiết bị truyền có thể thêm bit parity là 0 hoặc 1.
Phía nhận cấu hình parity giống như phía truyền, sau khi nhận đủ các bit sẽ kiểm tra parity có đúng hay không.

## 6.2 UART Hardware
STM32F1 có 3 khối USART: USART1 ở APB2 và USART2, USART3 ở APB1.
### 6.2.1 Cấu hình GPIO cho UART Hardware
STM32 đã cấu hình sẵn các chân dành cho chức năng USART. Khi sử dụng USART1, ta định nghĩa các chân đã được thiết lập sẵn:
```
#define UART1_TX	GPIO_Pin_9
#define UART1_RX	GPIO_Pin_10
#define UART1_GPIO	GPIOA
```
Cấu hình GPIO:
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIOInitStruct;

	GPIOInitStruct.GPIO_Pin = UART1_TX;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin = UART1_RX;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
}
```
### 6.2.2 Cấu hình UART
Tương tự các ngoại vi khác, các tham số Uart được cấu hình trong Struct **USART_InitTypeDef**:
- `USART_Mode`: Cấu hình chế độ hoạt động cho UART:
	- `USART_Mode_Tx`: Cấu hình truyền.
	- `USART_Mode_Rx`: Cấu hình nhận.
	- Có thể cấu hình cả 2 cùng lúc (song công).
- `USART_BaudRate**: Cấu hình tốc độ baudrate cho uart.
- `USART_HardwareFlowControl`: Cấu hình chế độ bắt tay cho uart.
- `USART_WordLength`: Cấu hình số bit mỗi lần truyền.
- `USART_StopBits`: Cấu hình số lượng stopbits.
- `USART_Parity`: Cấu hình bit kiểm tra chẳn, lẻ.
```
void UART_Config(){
		//Usart
	USARTInitStruct.USART_BaudRate = 9600;
	USARTInitStruct.USART_WordLength = USART_WordLength_8b;
	USARTInitStruct.USART_StopBits = USART_StopBits_1;
	USARTInitStruct.USART_Parity = USART_Parity_No;
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USARTInitStruct);
	USART_Cmd(USART1,ENABLE);
}
```
### 6.2.3 Các hàm thông dụng
- Hàm `USART_SendData(USART_TypeDef* USARTx, uint16_t Data)` truyền data từ UARTx. Data này đã được thêm bit chẵn/lẻ tùy cấu hình.
- Hàm `USART_ReceiveData(USART_TypeDef* USARTx)`, nhận data từ UARTx.
- Hàm `USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)` trả về trạng thái cờ `USART_FLAG` tương ứng:
	- `USART_FLAG_TXE`: Cờ truyền, set lên 1 nếu quá trình truyền hoàn tất.
	- `USART_FLAG_RXNE`: Cờ nhận, set lên 1 nếu quá trình nhận hoàn tất.
	- `USART_FLAG_IDLE`: Cờ báo đường truyền đang ở chế độ Idle.
	- `USART_FLAG_PE`: Cờ báo lỗi Parity.
### 6.2.4 Hàm truyền
- Bắt đầu truyền/nhận, UART xóa hết data trong thanh ghi DR để đảm bảo data đúng.
- Gửi đi từng byte data. Sau đó đợi cờ TXE bật lên.
Truyền 1 ký tự:
```
void UART_SendChar(USART_TypeDef *USARTx, char data)
{
	USARTx->DR = 0x00;
	USART_SendData(USARTx, data);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}
```

Truyền 1 chuỗi:
```
void UART_SendString(USART_TypeDef *USARTx, char *str)
{
	while(*str)
		{
		UART_SendChar(USARTx, *str);
		str++;
		}
}
```
### 6.2.5 Hàm nhận
- Đọc data từ bộ USART, chờ cờ RNXE bật lên.
- Đối với mảng dữ liệu, lặp lại quá trình cho từng byte.
```
char UART_ReceiveChar(USART_TypeDef *USARTx)
{
	USARTx->DR = 0x00;
	char tmp = 0x00;
	tmp = USART_ReceiveData(USARTx);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)==RESET);
	
	return tmp;
}
```
</details>

<details>
	<summary><strong>BÀI 7: NGẮT NGOÀI - NGẮT TIMER - NGẮT THUYỀN THÔNG</strong></summary>

# BÀI 7: NGẮT NGOÀI - NGẮT TIMER - NGẮT THUYỀN THÔNG
## 7.1 Ngắt ngoài
**External interrupt (EXTI)** hay còn gọi là ngắt ngoài là 1 sự kiện ngắt xảy ra khi có tín hiệu can thiệp từ bên ngoài, từ phần cứng, người sử dụng hay ngoại vi,… 

Để sử dụng được ngắt ngoài, ngoài bật clock cho GPIO tương ứng cần bật thêm clock cho AFIO.

Ngắt ngoài của chip STM32F103 bao gồm có 16 line ngắt riêng biệt:

![Picture13](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9a1de6c2-af85-474e-a8ec-c6285231acdf)

Ví dụ:
- Line0 nếu chúng ta đã chọn chân PA0 (chân 0 ở port A) làm chân ngắt thì tất cả các chân 0 ở các Port khác không được khai báo làm chân ngắt ngoài nữa
- Line1 nếu chúng ta chọn chân PB1 là chân ngắt thì tất cả chân 1 ở các Port khác không được khai báo làm chân ngắt nữa.

Các Line ngắt sẽ được phân vào các Vector ngắt tương ứng. Các Line ngắt của STM32F103 được phân bố vào các vector ngắt như sau:

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/00906ed3-92a3-47a7-ad68-30534ce70607)
### 7.1.1 Độ ưu tiên ngắt
Có 2 loại ưu tiên ngắt khác nhau trên MCU STM32F103C8T6 đó là **Preemption Priorities** và **Sub Priorities**:
- Mặc định thì ngắt nào có Preemtion Priority cao hơn thì sẽ được thực hiện trước.
- Khi nào 2 ngắt có cùng một mức Preemption Priority thì ngắt nào có Sub Priority cao hơn thì ngắt đó được thực hiện trước.
- Còn trường hợp 2 ngắt có cùng mức Preemption và Sub Priority luôn thì ngắt nào đến trước được thực hiện trước.
### 7.1.2 Cấu hình ngắt ngoài (EXTI)
Hàm `GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)` cấu hình chân ở chế độ sử dụng ngắt ngoài:
- `GPIO_PortSource`: Chọn Port để sử dụng làm nguồn cho ngắt ngoài.
- `GPIO_PinSource`: Chọn Pin để cấu hình.

Các tham số ngắt ngoài được cấu hình trong Struct **EXTI_InitTypeDef**, gồm:
- `EXTI_Line`: Chọn line ngắt.
- `EXTI_Mode`: Chọn Mode cho ngắt là Ngắt (thực thi hàm ngắt) hay Sự kiện (không thực thi)
- `EXTI_Trigger`: Cấu hình cạnh ngắt.
- `EXTI_LineCmd`: Cho phép ngắt ở Line đã cấu hình.
```
	EXTI_InitTypeDef EXTIInitStruct;

	EXTIInitStruct.EXTI_Line = EXTI_Line0;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTIInitStruct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTIInitStruct);
```
### 7.1.3 Cấu hình NVIC
Bộ NVIC cấu hình các tham số ngắt và quản lý các vecto ngắt. Các tham số được cấu hình trong **NVIC_InitTypeDef**, bao gồm:
- `NVIC_IRQChannel`: Cấu hình Line ngắt, Enable line ngắt tương ứng với ngắt sử dụng.
- `NVIC_IRQChannelPreemptionPriority`: Cấu hình độ ưu tiên của ngắt.
- `NVIC_IRQChannelSubPriority`: Cấu hình độ ưu tiên phụ.
- `NVIC_IRQChannelCmd`: Cho phép ngắt.
- 
Ngoài ra, `NVIC_PriorityGroupConfig();` cấu hình các bit dành cho **ChannelPreemptionPriority** và **ChannelSubPriority**: 
- `NVIC_PriorityGroup_0`: 0 bit pre-emption priority, 4 bit subpriority
- `NVIC_PriorityGroup_1`: 1 bit pre-emption priority, 3 bit subpriority
- `NVIC_PriorityGroup_2`: 2 bit pre-emption priority, 2 bit subpriority
- `NVIC_PriorityGroup_3`: 3 bit pre-emption priority, 1 bit subpriority
- `NVIC_PriorityGroup_4`: 4 bit pre-emption priority, 0 bit subpriority
```
NVIC_InitTypeDef NVICInitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICInitStruct);
```
### 7.1.4 Hàm phục vụ ngắt ngoài
- Ngắt trên từng line có hàm phục riêng của từng line, có tên cố định: `EXTIx_IRQHandler()` (x là line ngắt tương ứng). Hàm này sẽ được gọi khi có ngắt tương ứng trên Line xảy ra.
- Hàm `EXTI_GetITStatus(EXTI_Linex)` (x là Line ngắt): Kiểm tra cờ ngắt của line x tương ứng. Nếu chính xác Ngắt từ line x mới thực hiện các lệnh tiếp theo. 
- Hàm `EXTI_ClearITPendingBit(EXTI_Linex)`: Xóa cờ ngắt ở line `x`.

Trong hàm phục vụ ngắt ngoài, ta sẽ thực hiện:
- Kiểm tra ngắt đến từ line nào, có đúng là line cần thực thi hay không?
- Thực hiện các lệnh, các hàm.
- Xóa cờ ngắt ở line.
```
void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		//
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
```

## 7.2 Ngắt Timer
### 7.2.1 Cấu hình ngắt Timer
Sử dụng ngắt Timer, ta vẫn cấu hình các tham số trong **TIM_TimeBaseInitTypeDef** bình thường, riêng `TIM_Period`, đây là số chu kì mà timer sẽ ngắt. Ta tính toán và đặt giá trị để tạo khoảng thời gian ngắt mong muốn.

Cài đặt Period = 10-1 ứng với ngắt mỗi 1ms.

Hàm `TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE)` kích hoạt ngắt cho TIMERx tương ứng.
```
void TIM_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInitStruct.TIM_Period = 10-1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_Cmd(TIM2, ENABLE);
   	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
   	TIM_Cmd(TIM2, ENABLE);
}
```
### 7.2.2 Cấu hình NVIC
Cấu hình tương tự như ngắt ngoài EXTI, tuy nhiên `NVIC_IRQChannel` được đổi thành `TIM_IRQn` để khớp với line ngắt Timer.
```
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
```
### 7.2.3 Hàm phục vụ ngắt Timer
- Hàm phục vụ ngắt Timer được đặt tên : `TIMx_IRQHandler()` với `x` là timer tương ứng.	
- Bên trong hàm ngắt, ta kiểm tra cờ `TIM_IT_Update` bằng hàm `TIM_GetITStatus()`. Hàm này trả về giá trị kiểm tra xem timer đã tràn hay chưa.
- Sau khi thực hiện xong, gọi `TIM_ClearITPendingBit(TIMx, TIM_IT_Update);` để xóa cờ ngắt này.
```
uint16_t count;
void delay(int time){
	count=0; 
	while(count<time){}
}
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)){
		
	count++;
	// Clears the TIM2 interrupt pending bit
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);}
}
```

## 7.3 Ngắt truyền thông
STM32F1 hỗ trợ các ngắt cho các giao thức truyền nhận như SPI, I2C, UART…
Lấy ví dụ với UART ngắt nhận, các giao thức còn lại cũng sẽ có cách cấu hình tương tự.
### 7.3.1 Cấu hình ngắt UART
- Đầu tiên, các cấu hình tham số cho UART thực hiện bình thường.
- Trước khi cho phép UART hoạt động, cần kích hoạt ngắt UART bằng cách gọi hàm `USART_ITConfig();`
- Hàm `USART_ClearFlag(USART1, USART_IT_RXNE);` được gọi để xóa cờ ngắt ban đầu
```
void UART_Config(){
	USART_InitTypeDef UART_InitStruct;
	UART_InitStruct.USART_Mode = USART_Mode_Rx| USART_Mode_Tx;
	UART_InitStruct.USART_BaudRate = 9600;
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	UART_InitStruct.USART_Parity = USART_Parity_No;
	USART_Init(USART1, &UART_InitStruct);
	USART_ClearFlag(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}
```
### 7.3.2 Cấu hình NVIC
Ở NVIC, ta cấu hình tương tự như ngắt ngoài EXTI, ngắt Timer, tuy nhiên `NVIC_IRQChannel` được đổi thành `UARTx_IRQn` để khớp với line ngắt Timer.
```
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
```
### 7.3.3. Hàm phục vụ ngắt UART
- Hàm `USARTx_IRQHandler()` sẽ được gọi nếu xảy ra ngắt trên Line ngắt UART đã cấu hình. 
- Hàm `USART_GetITStatus` kiểm tra các cờ ngắt UART. Hàm này nhận 2 tham số là bộ USART và cờ tương ứng cần kiểm tra:
	- `USART_IT_RXNE`: Cờ ngắt nhận, cờ này set lên 1 khi bộ USART phát hiện data truyền tới.
	- `USART_IT_TXE`: Cờ ngắt truyền, cờ này set lên 1 khi USART truyền data đi.
- Có thể xóa cờ ngắt, gọi hàm `USART_ClearITPendingBit` để đảm bảo không còn ngắt trên line (thông thường cờ ngắt sẽ tự động xóa).
Trong hàm ngắt, ta thực hiện:
- Kiểm tra cờ ngắt từ bộ USART nào
- Thực hiện các hàm tương ứng
- Xóa cờ ngắt
```
void USART1_IRQHandler(){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
	{
		//
	}
	USART_ClearFlag(USART1, USART_IT_RXNE);
}
```
</details>

<details>
	<summary><strong>BÀI 8: ADC</strong></summary>

# BÀI 8: ADC
**ADC (Analog-to-Digital Converter)** là 1 mạch điện tử lấy điện áp tương tự làm đầu vào và chuyển đổi nó thành dữ liệu số (1 giá trị đại diện cho mức điện áp trong mã nhị phân).
![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/dc7a7df5-463d-4b44-a1a5-51c3e17109ea)

Khả năng chuyển đổi của ADC được quyết định bởi 2 yếu tố chính:
- **Độ phân giải**: Số bit mà ADC sử dụng để mã hóa tín hiệu. Có thể xem như là số mức mà tín hiệu tương tự được biểu diễn.
	- ADC có độ phân giải càng cao thì cho ra kết quả chuyển đổi càng chi tiết. 
![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/8540c0be-ba58-4259-9be3-c4eac58e1667)

- **Tần số/chu kì lấy mẫu**: Tốc độ/khoảng thời gian giữa 2 lần mã hóa. 
	- Tần số lấy mẫu càng lớn thì tín hiệu sau khi chuyển đổi sẽ có độ chính xác càng cao. Khả năng tái tạo lại tín hiệu càng chính xác.
	- Tần số lấy mẫu = 1/(Thời gian lấy mẫu + Thời gian chuyển đổi)
	- Tần số lấy mẫu phải lớn hơn tần số của tín hiệu ít nhất 2 lần để đảm bảo độ chính xác khi khôi phục lại tín hiệu.
 ![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/6d62278e-1d3e-4f46-bcba-4421ab2d29c6)

## 8.1 ADC trong STM32 
STM32F103C8 có 2 kênh ADC đó là ADC1 và ADC2, mỗi bộ có tối đa là 9 channel với nhiều mode hoạt động. 
Kết quả chuyển đổi được lưu trữ trong thanh ghi 16 bit. 
- Độ phân giải 12 bit.
- Có các ngắt hỗ trợ.
- Có thể điều khiển hoạt động ADC bằng xung Trigger.
- Thời gian chuyển đổi nhanh : 1us tại tần số 65Mhz.
- Có bộ DMA giúp tăng tốc độ xử lí.

## 8.2 Cấu hình ADC
Các bộ ADC được cấp xung từ RCC APB2, để bộ ADC hoạt động cần cấp xung cho cả ADC để tạo tần số lấy mẫu tín hiệu và cấp xung cho GPIO của Port ngõ vào.
```
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
```
### 8.2.1 Cấu hình GPIO cho ADC
ADC hỗ trợ rất nhiều kênh, mỗi kênh lấy tín hiệu từ các chân GPIO của các Port và từ các chân khác. Các chân GPIO dùng làm ngõ vào cho ADC sẽ được cấu hình Mode AIN (Analogue Input).
```
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```

### 8.2.2 Các chế độ ADC
- **Regular Conversion**:
	- **Single**: ADC chỉ đọc 1 kênh duy nhất, và chỉ đọc khi nào được yêu cầu.
	- **Single Continous**: ADC sẽ đọc một kênh duy nhất, nhưng đọc dữ liệu nhiều lần liên tiếp (Có thể được biết đến như sử dụng DMA để đọc dữ liệu và ghi vào bộ nhớ). 
	- **Scan: Multi-Channels**: Quét qua và đọc dữ liệu nhiều kênh, nhưng chỉ đọc khi nào được yêu cầu.
	- **Scan: Continous Multi-Channels Repeat**: Quét qua và đọc dữ liệu nhiều kênh, nhưng đọc liên tiếp nhiều lần giống như **Single Continous**. 
- **Injected Conversion**:
Trong trường hợp nhiều kênh hoạt động. Khi kênh có mức độ ưu tiên cao hơn có thể tạo ra một **Injected Trigger**. Khi gặp **Injected Trigger** thì ngay lập tức kênh đang hoạt động bị ngưng lại để kênh được ưu tiên kia có thể hoạt động.

### 8.2.3 Cấu hình ADC
Các tham số cấu hình cho bộ ADC được tổ chức trong Struct **ADC_InitTypeDef** bao gồm:
- `ADC_Mode`: Cấu hình chế độ hoạt động cho ADC là đơn kênh (Independent) hay đa kênh, ngoài ra còn có các chế độ ADC chuyển đổi tuần tự các kênh (regularly) hay chuyển đổi khi có kích hoạt (injected).
- `ADC_NbrOfChannel`: Số kênh ADC để cấu hình.
- `ADC_ContinuousConvMode`: Cấu hình bộ ADC có chuyển đổi liên tục hay không, Enable để cấu hình ADC chuyển đổi liên tục, nếu cấu hình Disable, ta phải gọi lại lệnh đọc ADC để bắt đầu quá trình chuyển đổi. 
- `ADC_ExternalTrigConv`: Enable để sử dụng tín hiệu trigger. 
- `ADC_ScanConvMode`: Cấu hình chế độ quét ADC lần lượt từng kênh. Enable nếu sử dụng chế độ quét này.
- `ADC_DataAlign`: Cấu hình căn lề cho data. Vì bộ ADC xuất ra giá trị 12 bit, được lưu vào biến 16 hoặc 32 bit nên phải căn lề các bit về trái hoặc phải.
Ngoài các tham số trên, cần cấu hình thêm thời gian lấy mẫu, thứ tự kênh ADC khi quét:
- `ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)`
	- `Rank`: Ưu tiên của kênh ADC.
	- `SampleTime`: Thời gian lấy mẫu tín hiệu.
```
void ADC_Config(){
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
```

## 8.3 Các hàm thông dụng
- `ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)`: Bắt đầu quá trình chuyển đổi.
- `ADC_GetConversionValue(ADC_TypeDef* ADCx)`: Đọc giá trị chuyển đổi được ở các kênh ADC tuần tự.
- `ADC_GetDualModeConversionValue(void)`: Trả về giá trị chuyển đổi cuối cùng của ADC1, ADC2 ở chế độ kép.

## 8.4 Thuật toán lọc Kalman
Giá trị đo được trên ADC có thể bị nhiễu, vọt lố do nhiều lý do khách quan về phần cứng.
![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9972cf65-ca7a-4235-8929-b02bedfd060a)

Các giá trị x- và Pk- được cập nhật liên tục từ các giá trị trước đó, từ đó ước tính được giá trị tiếp theo với hệ số Q do người dùng thiết lập và giá trị thực tế y.

Hàm thiết lập thông số ban đầu R, P, Q:
```
void SimpleKalmanFilter(float mea_e, float est_e, float q)
{
	_err_measure=mea_e;
	_err_estimate=est_e;
	_q = q;
}
```
Hàm tính giá trị qua bộ lọc Kalman:
```
float updateEstimate(float mea)
{
	_kalman_gain = _err_estimate/(_err_estimate + _err_measure);
	_current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
	_err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
	_last_estimate=_current_estimate;
	return _current_estimate;
}
```
Áp dụng để tính toán giá trị đo được từ ADC:
```
SimpleKalmanFilter(1, 2, 0.001);

while(1)
{
	val = ADC_GetConversionValue(ADC1);
			
	valupdate = (float)updateEstimate((float)val);
	Delay_Ms(100);
}
```
</details>

<details>
	<summary><strong>BÀI 9: DMA</strong></summary>

# BÀI 9: DMA
## 9.1 Hoạt động của Core
Cơ chế Master - Slave:
- CPU sẽ điều khiển việc trao đổi data giữa Peripheral (UART, I2C, SPI, ...) và bộ nhớ (RAM) qua các đường bus. 
- CPU phải lấy lệnh từ bộ nhớ (FLASH) để thực thi các lệnh của chương trình. 
- Vì vậy, khi cần truyền dữ liệu liên tục giữa Peripheral và RAM, CPU sẽ bị chiếm dụng, và không có thời gian làm các công việc khác, hoặc có thể gây miss dữ liệu khi transfer.
![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/73fa284c-e98b-4163-94a1-8b9332fc93cc)

## 9.2 DMA (Direct Memory Access)
**DMA** được sử dụng với mục đích truyền data với tốc độ cao từ thiết bị ngoại vi đến bộ nhớ cũng như từ bộ nhớ đến bộ nhớ. 
![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/d9800aa7-513a-42af-94b3-a4c7b359f890)

DMA có thể điều khiển data truyền từ :
- Bộ nhớ đến Peripheral 
- Ngược lại, Periph đến Bộ nhớ.
- Giữa 2 vùng nhớ.
- Không thông qua data bus của CPU. 
-> Giữ cho tài nguyên của CPU được rảnh rỗi cho các thao tác khác. Đồng thời tránh việc data nhận về từ ngoại vi bị mất mát.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9c3b9e9f-370c-4702-90ae-6a71f115dc59)

- Các Channel đều có thể được cấu hình riêng biệt.
- Mỗi Channel được kết nối để dành riêng cho tín hiệu DMA từ các thiết bị ngoại vi hoặc tín hiệu từ bên trong MCU.
- Có 4 mức ưu tiên có thể lập trình cho mỗi Channel.
- Kích thước data được sử dụng là 1 Byte, 2 Byte (Half Word) hoặc 4byte (Word)
- Hỗ trợ việc lặp lại liên tục Data.
- 5 cờ báo ngắt (DMA Half Transfer, DMA Transfer complete, DMA Transfer Error, DMA FIFO Error, Direct Mode Error).
- Quyền truy cập tới Flash, SRAM, APB1, APB2, AHB.
- Số lượng data có thể lập trình được lên tới 65535.
- Đối với DMA2, mỗi luồng đều hỗ trợ để chuyển dữ liệu từ bộ nhớ đến bộ nhớ.

STM32F1 có 2 bộ DMA với nhiều kênh, mỗi kênh có nhiều ngoại vi có thể dùng DMA.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/bd0aadd7-5a57-46a3-aa78-1b01cd762f62)

DMA có 2 chế độ hoạt động là **Normal** và **Circular**:
- **Normal mode**: DMA truyền dữ liệu cho tới khi truyền đủ 1 lượng dữ liệu giới hạn đã khai báo DMA sẽ dừng hoạt động. Muốn nó tiếp tục hoạt động thì phải khởi động lại.
- **Circular mode**: Khi DMA truyền đủ 1 lượng dữ liệu giới hạn đã khai báo thì nó sẽ truyền tiếp về vị trí ban đầu (Cơ chế như Ring buffer).

## 9.3 Cấu hình DMA
Cả 2 bộ DMA cần phải được cấp xung từ Bus AHB.
```
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
```
Ngoài ra cần phải cấp xung cho AFIO.
```
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
```

DMA có nhiều kênh, mỗi kênh phục vụ truyền DMA cho các ngoại vi riêng biệt. Cần cấu hình cho ngoại vi cần dùng DMA.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/bd0aadd7-5a57-46a3-aa78-1b01cd762f62)

Các tham số cho bộ DMA được cấu hình trong struct **DMA_InitTypeDef** bao gồm:
- `DMA_PeripheralBaseAddr`: Cấu hình địa chỉ của ngoại vi cho DMA. Đây là địa chỉ mà DMA sẽ lấy data hoặc truyền data tới cho ngoại vi.
- `DMA_MemoryBaseAddr`: Cấu hình địa chỉ vùng nhớ cần ghi/ đọc data .
- `DMA_DIR`: Cấu hình hướng truyền DMA, từ ngoại vi tới vùng nhớ hay từ vùng nhớ tới ngoại vi.
- `DMA_BufferSize`: Cấu hình kích cỡ buffer. Số lượng dữ liệu muốn gửi/nhận qua DMA.
- `DMA_PeripheralInc`: Cấu hình địa chỉ ngoại vi có tăng sau khi truyền DMA hay không.
- `DMA_MemoryInc`: Cấu hình địa chỉ bộ nhớ có tăng lên sau khi truyền DMA hay không.
- `DMA_PeripheralDataSize`: Cấu hình độ lớn data của ngoại vi.
- `DMA_MemoryDataSize`: Cấu hình độ lớn data của bộ nhớ.
- `DMA_Mode`: Cấu hình mode hoạt động.
- `DMA_Priority`: Cấu hình độ ưu tiên cho kênh DMA.
- `DMA_M2M`: Cấu hình sử dụng truyền từ bộ nhớ đếm bộ nhớ cho kênh DMA.
```
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_BufferSize = 35;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)buffer;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
```
Sau khi cấu hình cho DMA xong, chỉ cần gọi hàm `DMA_Cmd` cho ngoại vi tương ứng. Bộ DMA sẽ tự động truyền nhận data cũng như ghi dữ liệu vào vùng nhớ cụ thể. 
Ví dụ: Ngoại vi SPI1, RX nhận tương ứng với Channel2
```
	DMA_Init(DMA1_Channel2, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel2, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
```
</details>

<details>
	<summary><strong>BÀI 10: FLASH VÀ BOOTLOADER</strong></summary>

# BÀI 10: FLASH VÀ BOOTLOADER
## 10.1 Flash

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/d605231e-21d6-47ab-8bb5-188f63bdaf6f)

- STM32F1 không có EPROM mà chỉ được cung cấp sẵn 128/64Kb Flash. 
- Được chia nhỏ thành các Page, mỗi Page có kích thước 1Kb.
- Flash có giới hạn về số lần xóa/ghi.
- Trước khi ghi phải xóa Flash trước.
- Thường được dùng để lưu chương trình.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/f1c84e52-379a-4d81-98d6-3d7ba35b7537)

Thông thường chương trình sẽ được nạp vào vùng nhớ bắt đầu ở 0x08000000, vùng nhớ phía sau sẽ là trống và người dùng có thể lưu trữ dữ liệu ở vùng này.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/df0f830a-96ba-4f14-b566-e0aa27259bad)

### 10.1.1 Xóa Flash
- Mỗi lần ghi 2bytes hoặc 4bytes, tuy nhiên mỗi lần xóa phải xóa cả Page.
- Sơ đồ xóa FLash như hình:
	- Đầu tiên, kiểm tra cờ LOCK của Flash, nếu Cờ này đang được bật, Flash đang ở chế độ Lock và cần phải được Unlock trước khi sử dụng.
	- Sau khi FLash đã Unlock, cờ CR_PER được set lên 1.
	- Địa chỉ của Page cần xóa được ghi vào FAR.
	- Set bit CR_STRT lên 1 để bắt đầu quá trình xóa.
	- Kiểm tra cờ BSY đợi hoàn tất quá trình xóa.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/69757aa1-64de-4060-a7c0-68e200abb934)

### 10.1.2 Ghi Flash
- Flash có thể ghi theo 2/4bytes:
- Sơ đồ ghi FLash như hình:
	- Tương tự quá trình xóa, đầu tiên Cờ LOCK được kiểm tra.
	- Sau khi xác nhận đã Unlock, CỜ CR_PG được set lên 1.
	- Quá trình ghi dữ liệu vào địa chỉ tương ứng sẽ được thực thi.
	- Kiểm tra cờ BSY để đợi quá trình ghi hoàn tất.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/f4d6f875-25ff-4f6b-a895-43d645afd9e5)

### 10.1.3 Các hàm thông dụng
#### 10.1.3.1 Các hàm LOCK, UNLOCK Flash
- `void FLASH_Unlock(void)`: Hàm này Unlock cho tất cả vùng nhớ trong Flash.
- `void FLASH_UnlockBank1(void)`: Hàm này chỉ Unlock cho Bank đầu tiên. Vì SMT32F103C8T6 chỉ có 1 Bank duy nhất nên chức năng tương tự hàm trên.
- `void FLASH_UnlockBank2(void)`: Unlock cho Bank thứ 2.
- `void FLASH_Lock(void)`: Lock bộ điều khiển xóa Flash cho toàn bộ vùng nhớ Flash.
- `void FLASH_LockBank1(void)` và `void FLASH_LockBank2(void)`: Lock bộ điều khiển xóa Flash cho Bank 1 hoặc 2.

#### 10.1.3.2 Các hàm xóa Flash
- `FLASH_Status FLASH_EraseAllBank1Pages(void)`: Xóa tất cả các Page trong Bank 1 của Flash. 
- `FLASH_Status FLASH_EraseAllBank2Pages(void)`: Xóa tất cả các Page trong Bank 2 của Flash. 
- `FLASH_Status FLASH_EraseAllPages(void)`: Xóa toàn bộ Flash.
- `FLASH_Status FLASH_ErasePage(uint32_t Page_Address)`: Xóa 1 page cụ thể trong Flash, cụ thể là Page bắt đầu bằng địa chỉ Page_Address.

#### 10.1.3.3 Các hàm ghi Flash
- `FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)`:  Ghi dữ liệu vào vùng nhớ Address với kích thước mỗi 2 byte (Halfword).
- `FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)`: Ghi dữ liệu vào vùng nhớ Address với kích thước mỗi 4 byte (Word).
- `FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)`: hàm này trả về trạng thái của Flag. Ở bài này ta sẽ dùng hàm này để kiểm tra cờ FLASH_FLAG_BSY. Cờ này báo hiệu rằng Flash đang bận (Xóa/Ghi) nếu được set lên 1. 

Ví dụ:
Ghi data vào 1 Page trong Flash
```
void Flash_WriteInt(uint32_t address, uint16_t value){
	FLASH_Unlock();
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	FLASH_ProgramHalfWord(address, value);
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	FLASH_Lock();
}

void Flash_WriteNumByte(uint32_t address, uint8_t *data, int num){
	FLASH_Unlock();
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	uint16_t *ptr = (uint16_t*)data;
	for(int i=0; i<((num+1)/2); i++){
		FLASH_ProgramHalfWord(address+2*i, *ptr);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
		ptr++;
	}
	FLASH_Lock();
}
```
Xóa Flash
```
void Flash_Erase(uint32_t addresspage){
	FLASH_Unlock();
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	FLASH_ErasePage(addresspage);
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	FLASH_Lock();
}
```
</details>

	


































