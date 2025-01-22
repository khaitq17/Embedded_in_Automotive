<details>
	<summary><strong>BÀI 1: GPIO</strong></summary>
	
# Bài 1: GPIO

## 1.1 Cấp xung clock cho GPIO
Module RCC (Reset and Clock Control) cung cấp các hàm để cấu hình xung clock cho ngoại vi qua các Bus tương ứng.

![image](https://github.com/user-attachments/assets/3c717d69-8b75-47eb-83e6-d22c33b76a58)

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
  
![image](https://github.com/user-attachments/assets/623c061f-28e2-45fc-a316-bb815d17c5cd)

### 3.1.2 Quá trình truyền nhận dữ liệu
- Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận.
- Master cấp xung clock, với mỗi xung clock, 1 bit sẽ được truyền từ Master đến Slave và ngược lại.
- Các thanh ghi cập nhật giá trị và dịch 1 bit.
- Lặp lại quá trình đến khi truyền xong 8 bit thanh ghi.
  
![image](https://github.com/user-attachments/assets/b29a9242-dfd1-4ae5-93b0-bc7148d76398)

### 3.1.3 Các chế độ hoạt động
- Có 4 chế độ hoạt động phụ thuộc **Clock Polarity** (CPOL) và **Clock Phase** (CPHA).
- CPOL: 
	- CPOL = 0: Xung clock ban đầu ở mức 0.
    - CPOL = 1: Xung clock ban đầu ở mức 1.
- CPHA:
  	- CPHA = 0: Truyền bit trước rồi mới cấp xung.
    - CPHA = 1: Cấp xung trước rồi mới truyền bit.
          
| SPI Mode | CPOL | CPHA |
| :---: | :---: | :---: | 
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
      
![image](https://github.com/user-attachments/assets/ccf142fd-7e87-4b3b-9e6d-6f593026c209)

### 3.2.2 Quá trình truyền nhận dữ liệu
- Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
- Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.
- Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.
- ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.
- Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
- Tương tự cũng có ACK để chờ phản hồi.
- Stop: Điều kiện: Chân SDA lên mức 1 trước chân SCL.
   
![image](https://github.com/user-attachments/assets/8cbabb5d-13b1-4ac1-97b4-50e384364f61)

## 3.3 UART - Universal Asynchronous Receiver - Transmitter
- Là chuẩn giao tiếp **KHÔNG** đồng bộ.
- Hoạt động ở chế độ song công.
- Dùng Timer nội để phân biệt 2 bit liền kề.
- Tốc độ truyền: Baudrate = Số bit truyền/1s
### 3.3.1 Các đầu nối
- TX: Truyền.
- RX: Nhận.
      
![image](https://github.com/user-attachments/assets/1d18eb54-3e1d-47fb-9e3b-69b7911d5322)

### 3.3.2 Quá trình truyền nhận dữ liệu
- Start: 1 bit.
- Bit dữ liệu: 5 đến 9 bit.
- Bit chẵn lẻ:
	- Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
	- Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
- Stop: 1 đến 2 bit.
      
![image](https://github.com/user-attachments/assets/f505b51b-b638-4bce-ae50-c5dec678cf1d)

</details>

<details>
	<summary><strong>BÀI 4: GIAO TIẾP SPI</strong></summary>

# BÀI 4: GIAO TIẾP SPI
## 4.1 SPI Software
**SPI Software** là cách “mô phỏng” bằng việc tạo ra một giao thức truyền thông giống SPI nhưng chỉ sử dụng GPIO của vi điều khiển.
### 4.1.1 Cấu hình GPIO cho SPI Software
SPI dùng 4 chân để truyền nhận, gồm MISO, MOSI, CS và SCK.

![image](https://github.com/user-attachments/assets/351515bb-90be-4045-9d44-d91dc759b0d6)

- **SCK** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- **MISO** (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- **MOSI** (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.
- **CS** (Chip Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường CS tương ứng xuống mức 0 (Low).

Định nghĩa 4 chân sử dụng SPI:
```
#define SPI_SCK_Pin	GPIO_Pin_0
#define SPI_MISO_Pin	GPIO_Pin_1
#define SPI_MOSI_Pin 	GPIO_Pin_2
#define SPI_CS_Pin	GPIO_Pin_3
#define SPI_GPIO	GPIOA
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

![image](https://github.com/user-attachments/assets/b29a9242-dfd1-4ae5-93b0-bc7148d76398)

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
	uint8_t u8Mask = 0x80;	// 0b10000000
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
		u8Data = u8Data << 1;
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
	uint8_t dataReceive = 0x00;	//0b11000000
	uint8_t temp = 0x00, i=0;
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));		
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));		// Kiểm tra CS == 0
	for(i=0; i<8;i++)
    	{ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
		while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)) 
			temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);	// Đọc data trên MOSI
		dataReceive = dataReceive << 1;		// Dịch 1 bit
		dataReceive = dataReceive | temp;	// Ghi vào biến
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
	
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_SCK | SPI1_MISO | SPI1_MOSI;
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
	- `SPI_I2S_FLAG_BSY`: Cờ báo bận, set lên 1 khi SPI đang bận truyền nhận.

### 4.2.4 Hàm truyền
```
void SPI_Send1Byte(uint8_t data){
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);
   
    SPI_I2S_SendData(SPI1, data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == 0);
   
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);
}
```
### 4.2.5 Hàm nhận
```
uint8_t SPI_Receive1Byte(void){
    uint8_t temp;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == 1);
    temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == 0);
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

![image](https://github.com/user-attachments/assets/b42702da-8957-4765-b736-97a30d6f9220)

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

![image](https://github.com/user-attachments/assets/a89a8956-ca56-462b-bca0-97059feb973f)

```
#define WRITE_SDA_0 	GPIO_ResetBits(I2C_GPIO, I2C_SDA)	// Kéo chân SDA xuống 0
#define WRITE_SDA_1 	GPIO_SetBits(I2C_GPIO, I2C_SDA)		// Kéo chân SDA lên 1
#define WRITE_SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)	// Kéo chân SCL xuống 0
#define WRITE_SCL_1 	GPIO_SetBits(I2C_GPIO, I2C_SCL)		// Kéo chân SDA lên 1
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
- `I2C_Ack`: Cấu hình ACK, có sử dụng ACK hay không.
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
- Gửi/đọc các byte data, đợi truyền xong.
- Sau đó kết thúc bằng tín hiệu stop.

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

![image](https://github.com/user-attachments/assets/c04668d8-67a4-4dc4-929e-dd853a78bef4)

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

![image](https://github.com/user-attachments/assets/797ef8d7-adef-4e50-926b-892991ac094e)

- Hàm truyền sẽ truyền lần lượt 8 bit trong byte dữ liệu, sau khi tín hiệu start được gửi đi.
- Tạo start, delay 1 period time.
	- Truyền bit dữ liệu. mỗi bit truyền trong 1 period time.
	- Dịch 1 bit.
- Tạo stop, delay tương ứng với số bit stop.

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

![image](https://github.com/user-attachments/assets/9f4e7693-ae3e-4481-a700-51ac6a774fd8)

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
- `USART_BaudRate`: Cấu hình tốc độ baudrate cho uart.
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
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
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
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	
	return tmp;
}
```
</details>

<details>
	<summary><strong>BÀI 7: NGẮT NGOÀI - NGẮT TIMER - NGẮT TRUYỀN THÔNG</strong></summary>

# BÀI 7: NGẮT NGOÀI - NGẮT TIMER - NGẮT TRUYỀN THÔNG
## 7.1 Ngắt ngoài
**External interrupt (EXTI)** hay còn gọi là ngắt ngoài là 1 sự kiện ngắt xảy ra khi có tín hiệu can thiệp từ bên ngoài, từ phần cứng, người sử dụng hay ngoại vi,… 

Để sử dụng được ngắt ngoài, ngoài bật clock cho GPIO tương ứng cần bật thêm clock cho AFIO.

Ngắt ngoài của chip STM32F103 bao gồm có 16 line ngắt riêng biệt:

![image](https://github.com/user-attachments/assets/d6ce9f31-56ca-491d-b36f-787167c2cef1)

Ví dụ:
- Line0 nếu chúng ta đã chọn chân PA0 (chân 0 ở port A) làm chân ngắt thì tất cả các chân 0 ở các Port khác không được khai báo làm chân ngắt ngoài nữa
- Line1 nếu chúng ta chọn chân PB1 là chân ngắt thì tất cả chân 1 ở các Port khác không được khai báo làm chân ngắt nữa.

Các Line ngắt sẽ được phân vào các Vector ngắt tương ứng. Các Line ngắt của STM32F103 được phân bố vào các vector ngắt như sau:

![image](https://github.com/user-attachments/assets/0f96ad82-d598-4990-837a-e476723bcf1e)

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

Ngoài ra, `NVIC_PriorityGroupConfig();` cấu hình các bit dành cho **ChannelPreemptionPriority** và **ChannelSubPriority**: 
- `NVIC_PriorityGroup_0`: 0 bit pre-emption priority, 4 bit subpriority
- `NVIC_PriorityGroup_1`: 1 bit pre-emption priority, 3 bit subpriority
- `NVIC_PriorityGroup_2`: 2 bit pre-emption priority, 2 bit subpriority
- `NVIC_PriorityGroup_3`: 3 bit pre-emption priority, 1 bit subpriority
- `NVIC_PriorityGroup_4`: 4 bit pre-emption priority, 0 bit subpriority
```
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
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

![image](https://github.com/user-attachments/assets/69daa3d7-bde1-4fc1-812c-80ce7d54087d)

Khả năng chuyển đổi của ADC được quyết định bởi 2 yếu tố chính:
- **Độ phân giải**: Số bit mà ADC sử dụng để mã hóa tín hiệu. Có thể xem như là số mức mà tín hiệu tương tự được biểu diễn.
	- ADC có độ phân giải càng cao thì cho ra kết quả chuyển đổi càng chi tiết. 

![image](https://github.com/user-attachments/assets/e010fb51-ef09-4c31-8960-d0390e15ffd1)

- **Tần số/chu kì lấy mẫu**: Tốc độ/khoảng thời gian giữa 2 lần mã hóa. 
	- Tần số lấy mẫu càng lớn thì tín hiệu sau khi chuyển đổi sẽ có độ chính xác càng cao. Khả năng tái tạo lại tín hiệu càng chính xác.
	- Tần số lấy mẫu = 1/(Thời gian lấy mẫu + Thời gian chuyển đổi)
	- Tần số lấy mẫu phải lớn hơn tần số của tín hiệu ít nhất 2 lần để đảm bảo độ chính xác khi khôi phục lại tín hiệu.

 ![image](https://github.com/user-attachments/assets/f19d0aed-1e95-43e5-8a48-9c0a6fb3265b)

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

![image](https://github.com/user-attachments/assets/081da493-a4e2-473f-b910-3d7e438ace11)

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

![image](https://github.com/user-attachments/assets/1d1d03fc-8429-49af-899c-477f5bed8a22)

## 9.2 DMA (Direct Memory Access)
**DMA** được sử dụng với mục đích truyền data với tốc độ cao từ thiết bị ngoại vi đến bộ nhớ cũng như từ bộ nhớ đến bộ nhớ. 

![image](https://github.com/user-attachments/assets/8fec2a78-4b02-473e-a501-2dd7797a3ded)

DMA có thể điều khiển data truyền từ :
- Bộ nhớ đến Peripheral 
- Ngược lại, Periph đến Bộ nhớ.
- Giữa 2 vùng nhớ.
- Không thông qua data bus của CPU. 
-> Giữ cho tài nguyên của CPU được rảnh rỗi cho các thao tác khác. Đồng thời tránh việc data nhận về từ ngoại vi bị mất mát.

![image](https://github.com/user-attachments/assets/269b041f-229a-4e05-9ab0-408ceb51a524)

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

![image](https://github.com/user-attachments/assets/08a86ac5-5f06-4fa2-8190-f5d2c9eced80)

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

![image](https://github.com/user-attachments/assets/08a86ac5-5f06-4fa2-8190-f5d2c9eced80)

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

![image](https://github.com/user-attachments/assets/2ef7e549-88c6-4ce6-b640-75d25ae37c83)

- STM32F1 không có EPROM mà chỉ được cung cấp sẵn 128/64Kb Flash. 
- Được chia nhỏ thành các Page, mỗi Page có kích thước 1Kb.
- Flash có giới hạn về số lần xóa/ghi.
- Trước khi ghi phải xóa Flash trước.
- Thường được dùng để lưu chương trình.

![image](https://github.com/user-attachments/assets/efdc8879-cae4-4b1a-9b9a-a9d3a9d23bb2)

Thông thường chương trình sẽ được nạp vào vùng nhớ bắt đầu ở 0x08000000, vùng nhớ phía sau sẽ là trống và người dùng có thể lưu trữ dữ liệu ở vùng này.

![image](https://github.com/user-attachments/assets/b834f6d1-0648-4c1e-a95c-514f2fbad1e8)

### 10.1.1 Xóa Flash
- Mỗi lần ghi 2bytes hoặc 4bytes, tuy nhiên mỗi lần xóa phải xóa cả Page.
- Sơ đồ xóa FLash như hình:
	- Đầu tiên, kiểm tra cờ LOCK của Flash, nếu Cờ này đang được bật, Flash đang ở chế độ Lock và cần phải được Unlock trước khi sử dụng.
	- Sau khi FLash đã Unlock, cờ CR_PER được set lên 1.
	- Địa chỉ của Page cần xóa được ghi vào FAR.
	- Set bit CR_STRT lên 1 để bắt đầu quá trình xóa.
	- Kiểm tra cờ BSY đợi hoàn tất quá trình xóa.

![image](https://github.com/user-attachments/assets/c33ff15a-d27e-479b-9ee8-ee130bc95f54)

### 10.1.2 Ghi Flash
- Flash có thể ghi theo 2/4bytes:
- Sơ đồ ghi FLash như hình:
	- Tương tự quá trình xóa, đầu tiên Cờ LOCK được kiểm tra.
	- Sau khi xác nhận đã Unlock, CỜ CR_PG được set lên 1.
	- Quá trình ghi dữ liệu vào địa chỉ tương ứng sẽ được thực thi.
	- Kiểm tra cờ BSY để đợi quá trình ghi hoàn tất.

![image](https://github.com/user-attachments/assets/e045ed43-cb23-4da6-a8a4-7e13fbfd461b)

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
- 
Ví dụ: Xóa Flash
```
void Flash_Erase(uint32_t addresspage){
	FLASH_Unlock();
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	FLASH_ErasePage(addresspage);
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == 1);
	FLASH_Lock();
}
```
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

## 10.2 Bootloader
**Bootloader** là một ứng dụng có mục tiêu chính là nâng cấp hoặc sửa đổi phần mềm hệ thống mà không cần sự can thiệp của các công cụ nâng cấp chương trình cơ sở chuyên dụng. Bootloader có thể có nhiều chức năng, nhưng nó chủ yếu được sử dụng để quản lý ứng dụng. Nó cũng có thể sử dụng các giao thức khác nhau như UART, CAN, I2C, I2S, Ethernet hoặc USB để thiết lập giao tiếp và bắt đầu nâng cấp firmware.

Bootloader là chương trình chạy đầu tiên khi khởi động, thường gồm 2 loại:
- Bootloader do nhà sản xuất cung cấp
- Bootloader do người dùng tự viết

![image](https://github.com/user-attachments/assets/5144b079-48be-4199-8726-613c85eb7e6b)

Quá trình từ lúc cấp nguồn hoặc reset cho đến khi chạy hàm `main()`:

**Khi không có Bootloader**:
- Đầu tiên, MCU đọc giá trị BOOT0 và BOOT1 để quyết định bắt đầu đọc dữ liệu tại nơi nào của bộ nhớ.
- Địa chỉ bắt đầu của vùng nhớ đó sẽ được lưu vào thanh ghi **PC (Program Counter)** để tiến hành đọc lệnh từ đó.
- Lấy giá trị của ô nhớ đầu tiên để khởi tạo **MSP (Main Stack Pointer)**.
- Thanh ghi PC chạy đến ô nhớ tiếp theo, ô nhớ này chứa địa chỉ của **Reset_Handler**.
- Chương trình sẽ nhảy đến **Reset_Handler** để thực thi và làm các nhiệm vụ:
	- Khởi tạo hệ thống
	- Sao chép các dữ liệu (biến) từ Flash qua RAM
	- Gọi hàm `main()`

 ![image](https://github.com/user-attachments/assets/b0c1c8e2-7235-4418-9d8b-4c4589734694)


**Khi có Bootloader**:
- Sau khi Reset thì vi điều khiển nhảy đến `Reset_Handler()` mặc định ở địa chỉ 0x08000000 và nhảy đến hàm `main()` của chương trình Boot. 
- Chương trình Boot này nó sẽ lấy địa chỉ của chương trình ứng dụng muốn nhảy đến.
- Gọi hàm `Bootloader()`, hàm này sẽ set thanh ghi **SCB_VTOR** theo địa chỉ App muốn nhảy đến, `SCB➔VTOR = Firmware address`. 
- Sau đó gọi hàm Reset mềm (nhảy đến `Reset_Handler()`).
- Bây giờ Firmware mới bắt đầu chạy và Vi xử lý đã nhận diện `Reset_Handler()` ở địa chỉ mới nên dù có nhấn nút Reset thì nó vẫn chạy trong Application.

![image](https://github.com/user-attachments/assets/434575f7-32a6-4b9a-a57f-8fd6b858b45c)

```
#define ADDR_STR_BLINK	0x08008000

void Boot(void)
{
	// Thiết lập lại hệ thống clock
	RCC_DeInit();

	// Vô hiệu hóa các lỗi ngắt để tránh lỗi trong quá trình chuyển giao
	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

	// Cập nhật Stack Pointer từ bảng vector ngắt của chương trình chính
	__set_MSP(*(__IO uint32_t*) (ADDR_STR_BLINK));

	// Cập nhật thanh ghi SCB->VTOR để trỏ đến bảng vector ngắt của chương trình chính
	SCB->VTOR = ADDR_STR_BLINK;

	// Lấy địa chỉ Reset Handler của chương trình chính	
	uint32_t jumpAddress = *(__IO uint32_t*) (ADDR_STR_BLINK + 4);
	
	// Tạo con trỏ hàm đến Reset Handler
	void (*reset_handler)(void) = (void (*) (void)) jumpAddress;

	Nhảy vào Reset Handler của chương trình chính
	reset_handler();
}
```
</details>


<details>
	<summary><strong>BÀI 11: CAN</strong></summary>

# BÀI 11: CAN
**CAN (Controller Area Network)** là giao thức giao tiếp nối tiếp hỗ trợ mạnh cho những hệ thống điều khiển thời gian thực phân bố (distributed realtime control system).

CAN đặc biệt được ứng dụng nhiều trong ngành công nghiệp ô tô.

![image](https://github.com/user-attachments/assets/74754ed6-bd7b-450b-8cf8-849a2586449b)

Mỗi khối được điều khiển bởi 1 vi điều khiển riêng biệt, và thông qua CAN Bus để kết nối, giao tiếp với nhau. Và thời gian phản hồi của mỗi khối VĐK là cực kì nhanh.
## 11.1 Mạng CAN
CAN có đường dây dẫn gồm 2 dây CAN_H (CAN High) và CAN_L (CAN Low). Các thiết bị được nối chung trên 2 dây này và được gọi là 1 Node trong mạng.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/68f12553-8b0b-4dc6-a9ae-c60abc5c7ff9)

- **CAN Node**
Mạng CAN được tạo thành bởi 1 nhóm các Node.

1 Node bao gồm:
- Một thiết bị hỗ trợ xử lý điện áp trên Bus là CAN Transceiver.
- Một MCU có hỗ trợ CAN Controller (giao thức CAN). MCU ngoài việc nhận và xử lý data còn thực hiện chức năng của Node.  

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/d8958672-8ccc-4b34-b280-b34147b0286e)

- **CAN Transceiver**
Bộ Transceiver hỗ trợ cho 1 Node truyền dữ liệu lên Bus đồng thời nhận lại tín hiệu vừa truyền đi.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/a7b8a6cc-987e-4dd8-a7e9-f77bccd868b6)

- **Trạng thái "dominant" và "recessive"**
Can Bus định nghĩa 2 trạng thái điện áp là "dominant" (mức 0) và "recessive" (mức 1). Hai trạng thái này được xử lý bởi Transceiver của Node. Có 2 loại CAN tương ứng với các giá trị điện áp khác nhau.

**CAN low speed**

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/2c3c7083-c0a9-4804-872f-3bab731012a5)

|  | Trạng thái recessive | Trạng thái dominant|
|:--:|:--:|:--:|
| CAN_H | 1,75 V | 4 V |
| CAN_L | 3,25 V | 1 V |

**CAN high speed**

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/63dc0e5f-5d54-4dcb-883b-8404f71ed1d8)

|  | Trạng thái recessive | Trạng thái dominant|
|:--:|:--:|:--:|
| CAN_H | 2,5 V | 3,5 V |
| CAN_L | 2,5 V | 1,5 V |

- **Tính chất**

Vì tính chất vi sai trên đường truyền tín hiệu của CAN Bus, tín hiệu nhiễu sẽ ảnh hưởng đến giá trị điện áp ở trên cả CAN H và CAN L.

CAN Bus thường được **xoắn 2 dây vào nhau**. Khi đó tín hiệu nhiễu sẽ đều trên cả 2 dây CAN_H và CAN_L, lúc này nhiễu sẽ bị triệt tiêu.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/66ab8052-853f-4bb5-866c-7ccd06be6c5d)

## 11.2 Nguyên tắc hoạt động
- Dữ liệu truyền từ 1 Node bất ký trên CAN Bus không truyền địa chỉ của Node đó, cũng không truyền địa chỉ của Node muốn giao tiếp.
- Thay vào đó, dữ liệu truyền đi được gắn nhãn bởi 1 số nhận dạng (ID) duy nhất trên toàn mạng.

  Ví dụ: Có 8 bit ID: 0000 0000. Bit 1 liên quan đến động cơ, bit 2 liên quan đến phanh ABS, bit 3 liên quan đến bánh xe,... Khi set bit tương ứng lên 1 thì các Node liên quan sẽ xử lý thông điệp được truyền đi.
- Tất cả các Node khác đều nhận được thông điệp, mỗi Node tự kiểm tra mã ID để xem thông điệp có liên quan tới Node đó hay không. Nếu có liên quan quan, nó sẽ xử lý còn không thì bỏ qua.

**Tranh chấp trên Bus**

Giao thức CAN cho phép các Node khác nhau đưa dữ liệu cùng lúc. 

**Data Frame** và **Remote Frame** làm việc theo cơ chế phân xử quyền ưu tiên của tín hiệu. Vì thế cấu trúc của chúng có vùng phân xử quyền ưu tiên.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/de6a4bf9-bda3-4a99-9b1a-65df15455988)

Khi phân xử, bit ID có giá trị 0 sẽ được ưu tiên hơn.

## 11.3 Khung dữ liệu CAN
Trong mạng CAN, dữ liệu được truyền thông qua các khung dữ liệu (CAN Frame). Mỗi khung dữ liệu bao gồm nhiều trường khác nhau, từ việc chỉ định ID của thiết bị gửi cho đến việc chứa dữ liệu và thông tin kiểm tra lỗi. Mạng CAN hỗ trợ nhiều loại khung dữ liệu, mỗi loại có chức năng cụ thể gồm:
- Data Frame
- Remote Frame
- Error Frame
- Overload Frame
### 11.3.1 Data Frame
**Data Frame** 2 dạng là **Standard Frame** và **Extended Frame**.

Cấu trúc của 1 data frame:

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/89c30093-165e-4f6c-8569-34a28697412e)

#### 11.3.1.1 Standard Frame

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/3e34403a-df6b-4cda-9f24-11501ba23266)

- **Start Of Frame Field – SOF** (Trường bắt đầu): Độ dài 1 bit đầu của frame, giá trị Dominant báo hiệu bắt đầu của 1 frame.
- **Arbitration Field** (Trường xác định quyền ưu tiên): Có độ dài 12 bit, bao gồm 11 bit ID và 1 bit RTR.
    - **Bit RTR** (Remote Transmission Request): Dùng để phân biệt khung là **Data Frame** hay **Remote Frame** (0 - Data frame, 1 - Remote frame).
- **Control Field** (Trường điều khiển): Khung chuẩn gồm IDE, r0 và DLC (Data Length Code).
    - **Bit IDE** (Identifier Extension): Bit phân biệt giữa loại khung chuẩn và khung mở rộng: IDE = 0 - khung chuẩn.
    - **Bit r0** (bit dự trữ): Phải được truyền là Recessive Bit bởi bộ truyền nhưng bộ nhận không quan tâm đến giá trị bit này. 
    - **Bit DLC** (Data Length Code): Có độ dài 4 bit quy định số byte của trường dữ liệu của Data Frame, Chỉ được mang giá trị từ 0 đến 8.
- **Data Field** (Trường dữ liệu): Chứa data, có độ dài từ 0 đến 8 byte tùy vào giá trị của DLC ở trường điều khiển.
- **Cyclic Redundancy Check Field – CRC** (Trường kiểm tra): Gồm 16 bit và được chia làm 2 phần:
    - **CRC Sequence**: Gồm 15 bit CRC tuần tự, tính toán mã hóa dữ liệu. Khi nhận sẽ kiểm tra xem mã hóa đúng hay sai.
    - **CRC Delimiter**: Là một Recessive Bit làm nhiệm vụ phân cách trường CRC với trường phía sau.
- **Acknowledge Field – ACK** (Trường xác nhận): Có độ dài 2 bit và bao gồm 2 phần:
    - **ACK Slot**: Có độ dài 1 bit, Node truyền dữ liệu sẽ truyền bit này là Recessive. Khi một hoặc nhiều Node nhận chính xác giá trị thông điệp (không có lỗi và đã so sánh CRC Sequence trùng khớp) thì nó sẽ báo lại cho bộ truyền bằng cách truyền Dominant Bit ngay vị trí ACK Slot (tương tự việc kéo SDA trong I2C).
    - **ACK Delimiter**: Có độ dài 1 bit, nó luôn là một Recessive Bit ⇒ ACK Slot luôn được đặt giữa hai Recessive Bit là CRC Delimiter và ACK Delimiter.
- **End Of Frame Field – EOF** (Trường kết thúc): Thông báo kết thúc một Data Frame hay Remote Frame. Trường này gồm 7 Recessive Bit.

#### 11.3.1.2 Extended Frame

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/44e167be-b619-4060-a749-8d3a65424ace)

- **Start Of Frame Field – SOF** (Trường bắt đầu): Độ dài 1 bit đầu của frame, giá trị Dominant báo hiệu bắt đầu của 1 frame.
- **Arbitration Field** (Trường xác định quyền ưu tiên): Có độ dài 32 bit, bao gồm 29 bit ID, 1 bit SRR, 1 bit IDE và 1 bit RTR.
    - **Bit RTR** (Remote Transmission Request): Dùng để phân biệt khung là Data Frame hay Remote Frame (0- Data frame, 1- Remote frame).
    - **Bit SRR** (Substitute Remote Request): Chỉ có ở khung mở rộng, có giá trị là 1.
    - **Bit IDE** (Identifier Extension): Bit phân biệt giữa loại khung chuẩn và khung mở rộng: IDE = 1 - khung mở rộng. 
- **Control Field** (Trường điều khiển): Khung mở rộng gồm r1, r0 và DLC (Data Length Code).
    - **Bit r0, r1** (hai bit dự trữ): Phải được truyền là Recessive Bit bởi bộ truyền nhưng bộ nhận không quan tâm đến giá trị 2 bit này. 
    - **Bit DLC** (Data Length Code): Có độ dài 4 bit quy định số byte của trường dữ liệu của Data Frame, Chỉ được mang giá trị từ 0 đến 8.
- **Data Field** (Trường dữ liệu): Chứa data, có độ dài từ 0 đến 8 byte tùy vào giá trị của DLC ở trường điều khiển.
- **Cyclic Redundancy Check Field – CRC** (Trường kiểm tra): Gồm 16 bit và được chia làm 2 phần:
    - **CRC Sequence**: Gồm 15 bit CRC tuần tự, tính toán mã hóa dữ liệu. Khi nhận sẽ kiểm tra xem mã hóa đúng hay sai.
    - **CRC Delimiter**: Là một Recessive Bit làm nhiệm vụ phân cách trường CRC với trường phía sau.
- **Acknowledge Field – ACK** (Trường xác nhận): Có độ dài 2 bit và bao gồm 2 phần:
    - **ACK Slot**: Có độ dài 1 bit, Node truyền dữ liệu sẽ truyền bit này là Recessive. Khi một hoặc nhiều Node nhận chính xác giá trị thông điệp (không có lỗi và đã so sánh CRC Sequence trùng khớp) thì nó sẽ báo lại cho bộ truyền bằng cách truyền Dominant Bit ngay vị trí ACK Slot (tương tự việc kéo SDA trong I2C).
    - **ACK Delimiter**: Có độ dài 1 bit, nó luôn là một Recessive Bit ⇒ ACK Slot luôn được đặt giữa hai Recessive Bit là CRC Delimiter và ACK Delimiter.
- **End Of Frame Field – EOF** (Trường kết thúc): Thông báo kết thúc một Data Frame hay Remote Frame. Trường này gồm 7 Recessive Bit.

### 11.3.2 Remote Frame
Cấu trúc 1 **Remote Frame** tương tự Data frame, tuy nhiên Remote Frame có Bit RTR = 1 và không có trường data (Bit DLC = 0).

**Remote Frame** được sử dụng khi một node trên mạng CAN yêu cầu dữ liệu từ một node khác. Thay vì chứa dữ liệu thực, Remote Frame chứa ID của node cần yêu cầu, cùng với bit điều khiển RTR (Remote Transmission Request).

Remote Frame thường được sử dụng trong các hệ thống mà một node có thể yêu cầu thông tin từ một node khác mà không có dữ liệu nào được truyền ngay lập tức. Node nhận yêu cầu sẽ trả lời bằng một Data Frame chứa dữ liệu cần thiết.

### 11.3.3 Error Frame
**Error Frame** được sử dụng khi một node phát hiện ra lỗi trong quá trình truyền dữ liệu. Nó được gửi để thông báo cho các node khác rằng có lỗi đã xảy ra trên bus. Bất kỳ node nào phát hiện ra lỗi đều có thể gửi Error Frame.

**Error Frame** gồm 2 phần: **Error Flag** và **Error Delimiter**. 
- **Error Flag** là chuỗi từ 6 đến 12 bit dominant, báo hiệu lỗi.
- **Error Delimiter** là chuỗi 8 bit recessive, kết thúc Error Frame.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/e4a45569-6997-48b9-b452-e39a7dbdd8ee)

### 11.3.4 Overload Frame
**Overload Frame** được sử dụng để báo hiệu rằng một node đang trong trạng thái bận và không thể xử lý thêm thông điệp nào ngay lập tức. Điều này có thể xảy ra khi một node chưa xử lý xong thông điệp trước đó hoặc hệ thống quá tải.

Khi một node gửi Overload Frame, nó báo hiệu cho các node khác trên mạng rằng chúng cần dừng truyền thông trong một thời gian ngắn để giảm tải cho node đó.

## 11.4 Cấu hình CAN

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/cc24c5f1-9ac0-4e1a-a556-78707cb9aad5)

Trên STM32F1, CAN cần được cấp xung từ Bus APB1. Cấu hình cho 2 chân TX và RX của bộ CAN:

- RX (PA11): Mode In_Floating
- TX (PA12): Mode AF_PP
	
Các tham số cho CAN được cấu hình trong struct **CAN_InitTypeDef** bao gồm:
- `CAN_TTCM`: Chế độ giao tiếp được kích hoạt theo thời gian ấn định khoảng thời gian khi truyền message.
- `CAN_ABOM`: Quản lý ngắt bus tự động. Nếu trong quá trình truyền xảy ra lỗi, bus sẽ được ngắt. Bit này quy định việc CAN có quay về trạng thái bình thường hay không.
- `CAN_AWUM`: Chế độ đánh thức tự động. Nếu CAN hoạt động ở SleepMode, Bit này quy định việc đánh thức CAN theo cách thủ công hay tự động.
- `CAN_NART`: Không  tự động truyền lại. CAN sẽ thử lại để truyền tin nhắn nếu các lần thử trước đó không thành công. Nếu set bit này thì sẽ không truyền lại. Nên set bit khi sử dụng chung với **CAN_TTCM**, nếu không thì nên để = 0;
- `CAN_RFLM`: Chế độ khóa nhận FIFO. Chế độ khóa bộ đệm khi đầy.
- `CAN_TXFP`: Ưu tiên truyền FIFO. Đặt bit này =0, ưu tiên truyền các gói có ID thấp hơn. Nếu đặt lên 1, CAN sẽ ưu tiên các gói theo thứ tự trong bộ đệm.
- `CAN_Mode`: Chế độ CAN:
    - `CAN_Mode_Normal`: Gửi message thông thường.
    - `CAN_Mode_LoopBack`: Các message gửi đi sẽ được lưu vào bộ nhớ đệm.
    - `CAN_Mode_Silent`: Chế độ chỉ nhận.
    - `CAN_Mode_Silent_LoopBack`: Kết hợp giữa 2 mode trên.
- `CAN_Prescaler`: Cài đặt giá trị chia để tạo time quatum. 
    - `fCan` = `sysclk` / `CAN_Prescaler`
    - `1tq` = 1 / `fCan`
- `CAN_SJW`: Thời gian trễ phần cứng, tính theo tq.
- `CAN_BS1`: Thời gian đồng bộ đầu frame truyền, tính theo tq.
- `CAN_BS2`: Thời gian đồng bộ cuối frame truyền, tính theo tq.

Tốc độ truyền CAN = 1/(`SJW`+`BS1`+`BS2`)

## 11.5 CAN Mask & Filter
CAN hỗ trợ bộ lọc ID, giúp các Node có thể lọc ra ID từ các message trên Bus để quyết định sẽ nhận massge nào. Các tham số cho bộ lọc được cấu hình trong **CAN_FilterInitTypeDef**:
- `CAN_FilterNumber`: Chọn bộ lọc để dùng, từ 0-13.
- `CAN_FilterMode`: Chế độ bộ lọc: 
    - `IdMask`: Sử dụng mặt nạ bit để lọc ID.
    - `IdList`: Không sử dụng mặt nạ bit.
- `CAN_FilterScale`: Kích thước của bộ lọc, 32 hoặc 16 bit.
- `CAN_FilterMaskIdHigh` và `CAN_FilterMaskIdLow`: Giá trị cài đặt cho Mask, 32 bits.
- `CAN_FilterIdHigh` và `CAN_FilterIdLow`: Giá trị cài đặt cho bộ lọc, 32bits.
- `CAN_FilterFIFOAssignment`: Chọn bộ đệm cần áp dụng bộ lọc.
- `CAN_FilterActivation`: Kích hoạt bộ lọc ID.

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9396c7b8-ec3e-4b42-96ff-0e5b69c54539)

|Mask Bit n|Filter Bit n|Message Identifier Bit|Accept or Reject Bit n|
|:--:|:--:|:--:|:--:|
|0|x|x|Accept|
|1|0|0|Accept|
|1|0|1|Reject|
|1|1|0|Reject|
|1|1|1|Accept|

**Giá trị bộ lọc & Giá trị ID trong massage**
Thanh ghi chứa giá trị ID của gói tin:

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/0e78e5df-bfac-4696-9990-9a4b9ca9ce62)

Để áp dụng được Mask và ID cho gói tin với ID là stdID, cần setup 11 bit cao của Mask cùng như của Filter.

## 11.6 Truyền - nhận CAN
Để xác định được 1 gói tin, cần có **ID**, các bit **RTR**, **IDE**, **DLC** và tối đa 8 byte data như bài trước đã đề cập. Các thành phần này được tổ chức trong **CanRxMsg**.

Hàm truyền: `uint8_t CAN_Transmit(CAN_TypeDef CANx, CanTxMsg TxMessage)`:
- `CANx`: Bộ CAN cần dùng.
- `TxMessage`: Struct `CanRxMsg` cần truyền.

```
void CAN_TransmitData(uint8_t* data, uint8_t length)
{
	CanTxMsg TxMessage;
	
	TxMessage.StdId = 0x123;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = length;
	
	for (int i = 0; i < length; i++) {
		TxMessage.Data[i] = data[i];
	}
	
	uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
	
	while(CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok);
}
```

Gói tin nhận được sẽ được lưu dưới dạng **CanRxMsg** struct. Gồm các thành phần tương tự **CanTxMsg** của gói tin nhận được.

- Hàm `CAN_MessagePending(CAN_TypeDef CANx, uint8_t FIFONumber)`: Trả về số lượng gói tin đang đợi trong FIFO của bộ CAN. Dùng hàm này để kiểm tra xem bộ CAN có đang truyền nhận hay không, nếu FIFO trống thì có thể nhận.

- Hàm `CAN_Receive(CAN_TypeDef CANx, uint8_t FIFONumber, CanRxMsg RxMessage)`: Nhận về 1 gói tin từ bộ `CANx`, lưu vào `RxMessage`.

```
void CAN_ReceiveData(uint8_t* data)
{
	while(CAN_MessagePending(CAN1, CAN_FIFO0) < 1);
	CanRxMsg RxMessage;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	for (int i = 0; i < RxMessage.DLC; i++) {
		data[i] = RxMessage.Data[i];
	}
	
	CAN_FIFORelease(CAN1, CAN_FIFO0);	// Giải phóng FIFO
}
```
</details>


<details>
	<summary><strong>BÀI 12: LIN</strong></summary>

# BÀI 12: LIN
**LIN (Local Interconnect Network)** sinh ra nhằm giảm thiểu sự phức tạp và chi phí trong việc truyền thông giữa các thiết bị điện tử đơn giản trong xe.

LIN thường được sử dụng trong các ứng dụng điều khiển các chức năng không an toàn của xe như điều khiển cửa sổ, đèn, gương và điều hòa không khí.

![image](https://github.com/user-attachments/assets/cbc8cc92-18f4-4fc1-ba59-ff5ecf680d21)

## 12.1 Đặc điểm 

![image](https://github.com/user-attachments/assets/2c13c18b-019a-4e27-b77c-429f800d13b5)

- Mô hình **Master - Slave**.
- Node Slave chỉ phản hồi khi có yêu cầu từ node Master.
- Master quét yêu cầu tới tất cả các Slave để thăm dò thông tin.
- Master có dữ liệu từ Slave sẽ gửi lên bus CAN để đi tới các LIN khác.
- LIN sử dụng giao thức UART để truyền/nhận dữ liệu, với khung truyền dữ liệu sẽ là 1 start, 8 data, 1 hoặc 2 stop.

1 node LIN bao gồm 1 MCU có bộ LIN Controller và 1 LIN Transceiver.

![image](https://github.com/user-attachments/assets/0edeaadd-95e5-44ad-9f3d-58aea2d792d2)

## 12.2 Cấu trúc khung LIN
![image](https://github.com/user-attachments/assets/43bc1cbc-0f13-4123-8ca9-4644701be490)

- **Break** (>= 13 bit): Báo hiệu bắt đầu khung.
- **Sync** (8 bit): Byte đồng bộ hóa (0x55), điều chỉnh tốc độ truyền.	// 0x01010101
- **ID** (6 bit ID + 2 bit parity): Xác định loại dữ liệu và Slave cần phản hồi.
- **Data** (16 - 64 bit): Từ 2 đén 8 byte dữ liệu chứa nội dung chính của thông điệp.
- **Checksum** (8 bit): Phát hiện lỗi trong quá trình truyền thông.

## 12.3 Quá trình truyền thông LIN
- Master sẽ gửi Header cho các Slave trước.
- Nếu Master yêu cầu dữ liệu từ Slave thì Slave sẽ gửi lại Response cho Master.
- Nếu Master muốn gửi dữ liệu cho Slave thì sẽ gửi kèm Response cho Slave.

![image](https://github.com/user-attachments/assets/ebaa236d-f411-4cc2-8cb9-094290d59272)

- Slave sẽ xem ID có khớp với mình không, nếu có thì xử lý còn không thì bỏ qua.

## 12.4 Một số mạng thường dùng trên ô tô
|	| LIN | CAN | FlexRay | Ethernet |
|:-:|:---:|:---:|:-------:|:--------:|
|Tốc độ truyền|1 - 20 kbps|Lên đến 1 Mbps|Lên đến 10 Mbps|Lên đến 100 Mbps|
|Mô hình truyền|Master-Slave|Multi-Master|Multi-Master|Point-to-Point|
|Ứng dụng|Điều khiển thiết bị đơn giản|Hệ thống phức tạp|Hệ thống an toàn cao|Kết nối mạng tốc độ cao|
|Chi phí|Thấp|Trung bình|Cao|Cao|
|Thời gian thực|Không hỗ trợ|Hỗ trợ|Hỗ trợ chính xác|Hỗ trợ chính xác|

</details>

<details>
	<summary><strong>BÀI 13: AUTOSAR CLASSIC</strong></summary>

# BÀI 13: AUTOSAR CLASSIC
## 13.1 Khái niệm
- **AUTOSAR** (**AUT**omotive **O**pen **S**ystem **AR**chitecture) là một tiêu chuẩn toàn cầu cho phát triển phần mềm ô tô với mục tiêu là chuẩn hóa kiến trúc phần mềm cho các hệ thống điều khiển điện tử (ECU) trong ô tô, nhằm tăng tính khả chuyển, khả mở rộng và giảm chi phí phát triển.
- AUTOSAR được chia làm 2 loại:
    - **Autosar Classic**: Dành cho các hệ thống điều khiển điện tử (ECU) truyền thống với các yêu cầu thời gian thực khắt khe.
    - **Autosar Adaptive**: Dành cho các ứng dụng có tính năng linh hoạt, yêu cầu khả năng tính toán cao hơn, như lái xe tự động và kết nối. Nó hỗ trợ khả năng cập nhật phần mềm theo thời gian thực và phát triển dựa trên các tiêu chuẩn hiện đại như POSIX.

## 13.2 Kiến trúc phân lớp

![image](https://github.com/user-attachments/assets/92a2e075-1a0b-41f3-8601-84a8f5c23262)

### 13.2.1 Application layer
- **Application layer** (Lớp ứng dụng) là một thành phần quan trọng trong kiến ​​trúc phần mềm tổng thể của các hệ thống ô tô, bao gồm nhiều thành phần phần mềm (**Software Component - SWC**) thực hiện tập hợp các tác vụ để hoàn thành chức năng của xe.
- Mỗi SWC chứa nhiều chương trình, logic ứng dụng của hệ thống và không tương tác trực tiếp với phần cứng.
- SWC giao tiếp với phần cứng thông qua RTE.

### 13.2.2 Runtime Enviroment (RTE)
- **RTE** đóng vai trò quan trọng trong việc kết nối **Software Components (SWC)** và **Basic Software (BSW)** thông qua một kiến trúc trừu tượng.
- Khi SWC cần truy cập phần cứng, chúng không tương tác trực tiếp mà thông qua RTE. RTE sẽ xử lý việc ánh xạ này tới các thành phần BSW, bao gồm các lớp như **Microcontroller Abstraction Layer (MCAL)** và **ECU Abstraction Layer**.

- Ví dụ: 
    - 1 SWC cần gửi dữ liệu qua giao thức CAN: SWC sẽ gửi yêu cầu này qua cổng của mình, RTE sẽ ánh xạ tới lớp CAN Driver trong MCAL của BSW.
    - SWC cần đọc cảm biến: RTE sẽ ánh xạ yêu cầu này tới lớp Driver của cảm biến trong MCAL.

### 13.2.3 Basic software (BSW)
**Basic Software** (BSW) là một trong ba thành phần chính của kiến trúc AUTOSAR, đóng vai trò nền tảng để hỗ trợ phần mềm ứng dụng (SWC) hoạt động trên phần cứng. BSW cung cấp các dịch vụ cơ bản như quản lý phần cứng, giao tiếp, chẩn đoán, và các dịch vụ hệ thống.

BSW được chia thành 3 lớp chính:
- **Service Layer** (Lớp dịch vụ)
- **ECU Abstraction Layer** (Lớp trừu tượng hóa ECU)
- **Microcontroller Abstraction Layer - MCAL** (Lớp trừu tượng hóa vi điều khiển)

#### 13.2.3.1 Service Layer
Đây là lớp cao nhất trong BSW, cung cấp các dịch vụ hệ thống và tiện ích cho các phần mềm ứng dụng (SWC) và các lớp khác của BSW. Các dịch vụ này bao gồm quản lý thời gian thực, chẩn đoán, quản lý lỗi, quản lý nguồn, v.v.
- **OS (Operating System)**: Cung cấp các chức năng của hệ điều hành thời gian thực, bao gồm quản lý task, quản lý tài nguyên và đồng bộ hóa.
- **Memory Services**: Quản lý bộ nhớ không chỉ đọc/ghi mà còn các dịch vụ liên quan đến bảo mật dữ liệu, như Flash EEPROM.
- **Diagnostic Services**: Quản lý và xử lý chẩn đoán hệ thống, bao gồm chẩn đoán giao tiếp và xử lý lỗi.

#### 13.2.3.2 ECU Abstraction Layer
Lớp này cung cấp một giao diện trừu tượng cho tất cả các thiết bị ngoại vi và phần cứng cụ thể của ECU. Nó ẩn đi sự khác biệt về phần cứng của các thiết bị ngoại vi khác nhau và cung cấp một giao diện tiêu chuẩn cho các lớp bên trên (Service Layer và SWC).
- **I/O Hardware Abstraction (IoHwAb)**: đóng vai trò giao tiếp SWC và phần cứng, giúp trừu tượng hóa việc truy cập các thiết bị ngoại vi. Nó sẽ chuyển đổi các yêu cầu từ SWC thành các lệnh mà phần cứng có thể hiểu, giúp ứng dụng có thể dễ dàng tương thích với nhiều loại phần cứng khác nhau.
- **Communication Hardware Abstraction**: Hỗ trợ giao tiếp với các mạng truyền thông khác nhau, ví dụ như các giao thức truyền thông nội bộ ECU hoặc mạng xe.
- **Memory Hardware Abstraction**: Cung cấp giao diện để truy cập các loại bộ nhớ khác nhau mà không quan tâm đến cách thức thực hiện cụ thể.

#### 13.2.3.3 Microcontroller Abstraction Layer (MCAL)
Đây là lớp thấp nhất trong BSW, cung cấp giao diện trừu tượng để tương tác trực tiếp với các thành phần phần cứng của vi điều khiển, chẳng hạn như bộ xử lý trung tâm (CPU), các thiết bị ngoại vi tích hợp (như ADC, PWM, UART), và các bộ định thời (timer).
- **Microcontroller Drivers**: Điều khiển các tính năng cụ thể của vi điều khiển như bộ định thời (timer), bộ watchdog.
- **Memory Drivers**: Hỗ trợ giao tiếp, cấu hình với các bộ nhớ khác nhau như RAM, EEPROM, Flash, v.v.
- **Crypto Drivers**: Cung cấp các chức năng mã hóa hoặc giải mã.
- **Wireless Communication Drivers**: Hỗ trợ các giao thức truyền thông không dây như Bluetooth, WiFi.
- **Communication Drivers**: Hỗ trợ giao tiếp với CAN, LIN, SPI, Ethernet, v.v.
- **I/O Drivers**: Cung cấp cấu hình về ADC, PWM, ICU, v.v, .

### 13.2.3.4 Complex Device Driver
Đây là lớp chứa những tính răng riêng biệt của từng hãng xe.

Ví dụ:

![image](https://github.com/user-attachments/assets/f5a5465f-7dcd-4f93-9d94-6afe924867fb)

</details>






























