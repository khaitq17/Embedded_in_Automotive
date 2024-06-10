# Bài 2: GPIO

## 2.1. Cấp xung clock cho GPIO
Module RCC (Reset and Clock Control) cung cấp các hàm để cấu hình xung clock cho ngoại vi qua các Bus tương ứng.
![Picture1](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/daacd745-60e6-4499-bb41-86c84afb9edd)
```
RCC_APB1PeriphClockCmd

RCC_APB2PeriphClockCmd
	
RCC_AHBPeriphClockCmd
```
- Tham số đầu tiên là ngoại vi cần cấu hình clock.
- Tham số thứ 2 là giá trị quy định cấp (ENABLE) hay ngưng (DISABLE) xung clock cho ngoại vi đó.

## 2.2. Cấu hình GPIO
- Các tham số cho GPIO được tổ chức trong 1 struct **GPIO_InitTypeDef**:
	- GPIO_Pin: Chọn chân
 	- GPIO_Mode: Chọn chế độ
    - GPIO_Speed: Chọn tốc độ
- Dùng hàm **GPIO_Init** để khởi tạo GPIO:
    - GPIO_TypeDef: GPIO cần cấu hình
    - &GPIO_InitStruct: Con trỏ trỏ tới biến TypeDef vừa được khởi tạo

## 2.3. Một số hàm thông dụng trong GPIO
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

# BÀI 3: Ngắt và Timer
## 3.1. Ngắt
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

## 3.2. Timer
- Timer là 1 mạch digital logic có vai trò đếm mỗi chu kỳ clock (đếm lên hoặc đếm xuống).
Cấu hình Timer:    
- Cấp xung cho Timer
- Cấu hình cho Timer trong struct **TIM_TimeBaseInitTypeDef**:
    - TIM_ClockDivision: Chia tần số
 	  - TIM_Prescaler: Quy định sau bao nhiêu dao động thì đếm lên 1 lần
  	- TIM_Period: Quy định thời gian 1 chu kỳ
   	- TIM_Mode: Chọn chế độ (Đếm lên hoặc đếm xuống)

# BÀI 4: Các chuẩn giao tiếp cơ bản
## 4.1. SPI - Serial Peripheral Interface
- Là chuẩn giao tiếp đồng bộ.
- Hoạt động ở chế độ song công (Có thể truyền - nhận cùng thời điểm).
### 4.1.1. Các đầu nối
- **SCK** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- **MISO** (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- **MOSI** (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.
- **SS** (Slave Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).
  
![Picture2](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/59a2f364-6d36-4043-927f-1324d5c19944)
### 4.1.2. Quá trình truyền nhận dữ liệu
- Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận.
- Master cấp xung clock, với mỗi xung clock, 1 bit sẽ được truyền từ Master đến Slave và ngược lại.
- Các thanh ghi cập nhật giá trị và dịch 1 bit.
- Lặp lại quá trình đến khi truyền xong 8 bit thanh ghi.
  
![Picture3](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/f984fc6c-1364-4c0d-8566-e4bdb93302db)
### 4.1.3. Các chế độ hoạt động
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

## 4.2. I2C - Inter-Integrated Circuit
- Là chuẩn giao tiếp đồng bộ.
- Hoạt động ở chế độ bán song công.
- Một Master giao tiếp được với nhiều Slave.
### 4.2.1. Các đầu nối
- **SDA** (Serial Data): Đường truyền cho Master và Slave để gửi và nhận dữ liệu.
- **SCL** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
      
![Picture4](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/89a016e7-34fa-443d-8736-0102a2a6a62a)
### 4.2.2. Quá trình truyền nhận dữ liệu
- Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
- Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.
- Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.
- ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.
- Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
- Tương tự cũng có ACK để chờ phản hồi.
- Stop: Điều kiện: Chân SDA lên mức 1 trước chân SCL.
   
 ![Picture5](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/d5ba043a-1d63-4de2-a7a6-bdb783e4acee)

## 4.3. UART - Universal Asynchronous Receiver - Transmitter
- Là chuẩn giao tiếp **KHÔNG** đồng bộ.
- Hoạt động ở chế độ song công.
- Dùng Timer nội để phân biệt 2 bit liền kề.
- Tốc độ truyền: Baudrate = Số bit truyền/1s
### 4.3.1. Các đầu nối
- TX: Truyền.
- RX: Nhận.
      
![Picture6](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9a52fd6e-eb76-43f4-aa81-9cce344ae3a7)
### 4.3.2. Quá trình truyền nhận dữ liệu
- Start: 1 bit.
- Bit dữ liệu: 5 đến 9 bit.
- Bit chẵn lẻ:
	- Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
	- Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
- Stop: 1 đến 2 bit.
      
![Picture7](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/56943f89-3f0a-4785-b944-51966027ff2b)

# BÀI 5: Giao tiếp SPI
## 5.1. SPI Software
SPI Software là cách “mô phỏng” bằng việc tạo ra một giao thức truyền thông giống SPI nhưng chỉ sử dụng GPIO của vi điều khiển.
### 5.1.1. Cấu hình GPIO cho SPI Software
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
### 5.1.2. Khởi tạo các chân cho SPI
```
void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}
```
### 5.1.3. Hàm truyền
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
### 5.1.4. Hàm nhận
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

## 5.2. SPI Hardware
Trên mỗi dòng vi điều khiển khác nhau module SPI sẽ được tích hợp, điều khiển bởi các thanh ghi, phần cứng, IO khác nhau gọi là SPI cứng (SPI Hardware). STM32F1 có 2 khối SPI: SPI1 ở APB2 và SPI2 ở APB1.
### 5.2.1. Cấu hình GPIO cho SPI Hardware
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
### 5.2.2. Cấu hình SPI
Tương tự các ngoại vi khác, các tham số SPI được cấu hình trong Struct **SPI_InitTypeDef**:
- **SPI_Mode**: Quy định chế độ hoạt động của thiết bị SPI. 
- **SPI_Direction**: Quy định kiểu truyền của thiết bị.
- **SPI_BaudRatePrescaler**: Hệ số chia clock cấp cho Module SPI.
- **SPI_CPOL**: Cấu hình cực tính của SCK . Có 2 chế độ:
	- SPI_CPOL_Low: Cực tính mức 0 khi SCK không truyền xung.
	- SPI_CPOL_High: Cực tính mức 1 khi SCK không truyền xung.
- **SPI_CPHA**: Cấu hình chế độ hoạt động của SCK. Có 2 chế độ:
	- SPI_CPHA_1Edge: Tín hiệu truyền đi ở cạnh xung đầu tiên.
	- SPI_CPHA_2Edge: Tín hiệu truyền đi ở cạnh xung thứ hai.
- **SPI_DataSize**: Cấu hình số bit truyền. 8 hoặc 16 bit.
- **SPI_FirstBit**: Cấu hình chiều truyền của các bit là MSB hay LSB.
- **SPI_CRCPolynomial**: Cấu hình số bit CheckSum cho SPI.
- **SPI_NSS**: Cấu hình chân SS là điều khiển bằng thiết bị hay phần mềm.

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
### 5.2.3. Các hàm thông dụng
- Hàm **SPI_I2S_SendData(SPI_TypeDef SPIx, uint16_t Data)**, tùy vào cấu hình datasize là 8 hay 16 bit sẽ truyền đi 8 hoặc 16 bit dữ liệu. Hàm nhận 2 tham số là bộ SPI sử dụng và data cần truyền.
- Hàm **SPI_I2S_ReceiveData(SPI_TypeDef SPIx)** trả về giá trị đọc được trên SPIx. Hàm trả về 8 hoặc 16 bit data.
- Hàm **SPI_I2S_GetFlagStatus(SPI_TypeDef SPIx, uint16_t SPI_I2S_FLAG)** trả về giá trị 1 cờ trong thanh ghi của SPI. Các cờ thường được dùng:
	- SPI_I2S_FLAG_TXE: Cờ báo truyền, cờ này sẽ set lên 1 khi truyền xong data trong buffer.
	- SPI_I2S_FLAG_RXNE: Cờ báo nhận, cờ này set lên 1 khi nhận xong data.
	- SPI_I2S_FLAG_BSY: Cờ báo bận,set lên 1 khi SPI đang bận truyền nhận.

### 5.2.4. Hàm truyền
```
void SPI_Send1Byte(uint8_t data){
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);
   
    SPI_I2S_SendData(SPI1, data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0);
   
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);
}
```
### 5.2.5. Hàm nhận
```
uint8_t SPI_Receive1Byte(void){
    uint8_t temp;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==1);
    temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==0);
    return temp;
}
```

# BÀI 6: Giao tiếp I2C
## 6.1. I2C Software
### 6.1.1. Cấu hình GPIO cho I2C Software
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
### 6.1.2. Cấu hình I2C
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
### 6.1.3. Hàm truyền
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
### 6.1.4. Hàm nhận
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

## 6.2. I2C Hardware
STM32F1 có 2 khối I2C: I2C1 và I2C2 ở APB1.
### 6.2.1. Cấu hình GPIO cho I2C Hardware
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
### 6.2.2. Cấu hình I2C
Tương tự các ngoại vi khác, các tham số I2C được cấu hình trong Struct **I2C_InitTypeDef**:
- **I2C_Mode**: Cấu hình chế độ hoạt động cho I2C:
	- I2C_Mode_I2C: Chế độ I2C FM(Fast Mode);
	- I2C_Mode_SMBusDevice&I2C_Mode_SMBusHost: Chế độ SM (Slow Mode).
- **I2C_ClockSpeed**: Cấu hình clock cho I2C, tối đa 100khz với SM và 400khz ở FM.
- **I2C_DutyCycle**: Cấu hình chu kì nhiệm vụ của xung:
	- I2C_DutyCycle_2: Thời gian xung thấp/xung cao = 2;
	- I2C_DutyCycle_16_9: Thời gian xung thấp/xung cao = 16/9;
- **I2C_OwnAddress1**: Cấu hình địa chỉ Slave.
- **I2C_Ack**: Cấu hình ACK, có sử dụng ACK hay không.
- **I2C_AcknowledgedAddress**: Cấu hình số bit địa chỉ (7 hoặc 10 bit)
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
### 6.2.3. Các hàm thông dụng
- Hàm **I2C_Send7bitAddress(I2C_TypeDef I2Cx, uint8_t Address, uint8_t I2C_Direction)**, gửi đi 7 bit address để xác định slave cần giao tiếp. Hướng truyền được xác định bởi I2C_Direction để thêm bit RW.
- Hàm **I2C_SendData(I2C_TypeDef I2Cx, uint8_t Data)** gửi đi 8 bit data.
- Hàm **I2C_ReceiveData(I2C_TypeDef I2Cx)** trả về 8 bit data.
- Hàm **I2C_CheckEvent(I2C_TypeDef I2Cx, uint32_t I2C_EVENT)** trả về kết quả kiểm tra I2C_EVENT tương ứng:
	- I2C_EVENT_MASTER_MODE_SELECT: Đợi Bus I2C về chế độ rảnh.
	- I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: Đợi xác nhận của Slave với yêu cầu ghi của Master.
	- I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED: Đợi xác nhận của Slave với yêu cầu đọc của Master.
	- I2C_EVENT_MASTER_BYTE_TRANSMITTED: Đợi truyền xong 1 byte data từ Master.
	- I2C_EVENT_MASTER_BYTE_RECEIVED: Đợi Master nhận đủ 1 byte data.
### 6.2.4. Hàm truyền
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
### 6.2.5. Hàm nhận
```
uint8_t Read_I2C_Data()
{
	uint8_t data = I2C_ReceiveData(I2C1);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return data;
}
```

# BÀI 7: Giao tiếp UART
## 7.1. UART Software
### 7.1.1. Cấu hình GPIO cho UART Software

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
### 7.1.2. Baudrate
Tốc độ baudrate được xác định bởi thời gian truyền đi 1 bit. Ta dùng tốc độ phổ thông 9600, ứng với mỗi bit là 105us.
Baaurate = 9600bits/s >> 0.10467ms for 1 bit = 104,67 us
=>> time delay ~~105 us
```
#define BRateTime 105
```
### 7.1.3. Cấu hình UART
```
void UART_Config(){
	GPIO_SetBits(UART_GPIO, TX_Pin);
	delay_us(1);
}
```
### 7.1.4. Hàm truyền
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
### 7.1.5. Hàm nhận
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
### 7.1.6. Parity
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

## 7.2. UART Hardware
STM32F1 có 3 khối USART: USART1 ở APB2 và USART2, USART3 ở APB1.
### 7.2.1. Cấu hình GPIO cho UART Hardware
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
### 7.2.2. Cấu hình UART
Tương tự các ngoại vi khác, các tham số Uart được cấu hình trong Struct **USART_InitTypeDef**:
- **USART_Mode**: Cấu hình chế độ hoạt động cho UART:
	- USART_Mode_Tx: Cấu hình truyền.
	- USART_Mode_Rx: Cấu hình nhận.
	- Có thể cấu hình cả 2 cùng lúc (song công).
- **USART_BaudRate**: Cấu hình tốc độ baudrate cho uart.
- **USART_HardwareFlowControl**: Cấu hình chế độ bắt tay cho uart.
- **USART_WordLength**: Cấu hình số bit mỗi lần truyền.
- **USART_StopBits**: Cấu hình số lượng stopbits.
- **USART_Parity**: cấu hình bit kiểm tra chẳn, lẻ.
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
### 7.2.3. Các hàm thông dụng
- Hàm **USART_SendData(USART_TypeDef USARTx, uint16_t Data)**, truyền data từ UARTx. Data này đã được thêm bit chẵn/lẻ tùy cấu hình.
- Hàm **USART_ReceiveData(USART_TypeDef USARTx)**, nhận data từ UARTx.
- Hàm **USART_GetFlagStatus(USART_TypeDef USARTx, uint16_t USART_FLAG)** trả về trạng thái cờ USART_FLAG tương ứng:
	- USART_FLAG_TXE: Cờ truyền, set lên 1 nếu quá trình truyền hoàn tất.
	- USART_FLAG_RXNE: Cờ nhận, set lên 1 nếu quá trình nhận hoàn tất.
	- USART_FLAG_IDLE: Cờ báo đường truyền đang ở chế độ Idle.
	- USART_FLAG_PE: Cờ báo lỗi Parity.
### 7.2.4. Hàm truyền
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
### 7.2.5. Hàm nhận
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

# BÀI 8: Ngắt ngoài - Ngắt Timer - Ngắt truyền thông
## 8.1. Ngắt ngoài
External interrupt (EXTI) hay còn gọi là ngắt ngoài là 1 sự kiện ngắt xảy ra khi có tín hiệu can thiệp từ bên ngoài, từ phần cứng, người sử dụng hay ngoại vi,… 
Để sử dụng được ngắt ngoài, ngoài bật clock cho GPIO tương ứng cần bật thêm clock cho AFIO.
Ngắt ngoài của chip STM32F103 bao gồm có 16 line ngắt riêng biệt:

![Picture13](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9a1de6c2-af85-474e-a8ec-c6285231acdf)
Ví dụ:
- Line0 nếu chúng ta đã chọn chân PA0 (chân 0 ở port A) làm chân ngắt thì tất cả các chân 0 ở các Port khác không được khai báo làm chân ngắt ngoài nữa
- Line1 nếu chúng ta chọn chân PB1 là chân ngắt thì tất cả chân 1 ở các Port khác không được khai báo làm chân ngắt nữa.
Các Line ngắt sẽ được phân vào các Vector ngắt tương ứng. Các Line ngắt của STM32F103 được phân bố vào các vector ngắt như sau:

![image](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/00906ed3-92a3-47a7-ad68-30534ce70607)
### 8.1.1. Độ ưu tiên ngắt
Có hai loại ưu tiên ngắt khác nhau trên MCU STM32F103C8T6 đó là Preemption Priorities và Sub Priorities:
– Mặc định thì ngắt nào có Preemtion Priority cao hơn thì sẽ được thực hiện trước.
– Khi nào 2 ngắt có cùng một mức Preemption Priority thì ngắt nào có Sub Priority cao hơn thì ngắt đó được thực hiện trước.
– Còn trường hợp 2 ngắt có cùng mức Preemption và Sub Priority luôn thì ngắt nào đến trước được thực hiện trước.
### 8.1.2. Cấu hình ngắt ngoài (EXTI)
Hàm **GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)** cấu hình chân ở chế độ sử dụng ngắt ngoài:
- GPIO_PortSource: Chọn Port để sử dụng làm nguồn cho ngắt ngoài.
- GPIO_PinSource: Chọn Pin để cấu hình.

Các tham số ngắt ngoài được cấu hình trong Struct **EXTI_InitTypeDef**, gồm:
- **EXTI_Line**: Chọn line ngắt.
- **EXTI_Mode**: Chọn Mode cho ngắt là Ngắt(thực thi hàm ngắt) hay Sự kiện(Không thực thi)
- **EXTI_Trigger**: Cấu hình cạnh ngắt.
- **EXTI_LineCmd**: Cho phép ngắt ở Line đã cấu hình.
```
	EXTI_InitTypeDef EXTIInitStruct;

	EXTIInitStruct.EXTI_Line = EXTI_Line0;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTIInitStruct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTIInitStruct);
```
### 8.1.3. Cấu hình NVIC
Bộ NVIC cấu hình các tham số ngắt và quản lý các vecto ngắt. Các tham số được cấu hình trong **NVIC_InitTypeDef**, bao gồm:
- **NVIC_IRQChannel**: Cấu hình Line ngắt, Enable line ngắt tương ứng với ngắt sử dụng.
- **NVIC_IRQChannelPreemptionPriority**: Cấu hình độ ưu tiên của ngắt.
- **NVIC_IRQChannelSubPriority**: Cấu hình độ ưu tiên phụ.
- **NVIC_IRQChannelCmd**: Cho phép ngắt.
Ngoài ra, **NVIC_PriorityGroupConfig();** cấu hình các bit dành cho **ChannelPreemptionPriority** và **ChannelSubPriority**: 
- NVIC_PriorityGroup_0: 0 bits for pre-emption priority 4 bits for subpriority
- NVIC_PriorityGroup_1: 1 bits for pre-emption priority 3 bits for subpriority
- NVIC_PriorityGroup_2: 2 bits for pre-emption priority 2 bits for subpriority
- NVIC_PriorityGroup_3: 3 bits for pre-emption priority 1 bits for subpriority
- NVIC_PriorityGroup_4: 4 bits for pre-emption priority 0 bits for subpriority
```
NVIC_InitTypeDef NVICInitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICInitStruct);
```
### 8.1.4. Hàm phục vụ ngắt ngoài
- Ngắt trên từng line có hàm phục riêng của từng line, có tên cố định: **EXTIx_IRQHandler()** (x là line ngắt tương ứng). Hàm này sẽ được gọi khi có ngắt tương ứng trên Line xảy ra.
- Hàm **EXTI_GetITStatus(EXTI_Linex)** (x là Line ngắt): Kiểm tra cờ ngắt của line x tương ứng. Nếu chính xác Ngắt từ line x mới thực hiện các lệnh tiếp theo. 
- Hàm **EXTI_ClearITPendingBit(EXTI_Linex)**: Xóa cờ ngắt ở line x.

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

## 8.2. Ngắt Timer
### 8.2.1. Cấu hình ngắt Timer
Sử dụng ngắt timer, ta vẫn cấu hình các tham số trong **TIM_TimeBaseInitTypeDef** bình thường, riêng **TIM_Period**, đây là số chu kì mà timer sẽ ngắt. Ta tính toán và đặt giá trị để tạo khoảng thời gian ngắt mong muốn.
Cài đặt Period = 10-1 ứng với ngắt mỗi 1ms.
Hàm **TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE)** kích hoạt ngắt cho TIMERx tương ứng.
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
### 8.2.2. Cấu hình NVIC
Cấu hình tương tự như ngắt ngoài EXTI, tuy nhiên **NVIC_IRQChannel** được đổi thành **TIM_IRQn** để khớp với line ngắt Timer.
```
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
```
### 8.2.3. Hàm phục vụ ngắt Timer
- Hàm phục vụ ngắt Timer được đặt tên : **TIMx_IRQHandler()** với x là timer tương ứng.	
- Bên trong hàm ngắt, ta kiểm tra cờ **TIM_IT_Update** bằng hàm **TIM_GetITStatus()**. Hàm này trả về giá trị kiểm tra xem timer đã tràn hay chưa.
- Sau khi thực hiện xong, gọi **TIM_ClearITPendingBit(TIMx, TIM_IT_Update);** để xóa cờ ngắt này.
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

## 8.3. Ngắt truyền thông
STM32F1 hỗ trợ các ngắt cho các giao thức truyền nhận như SPI, I2C, UART…
Lấy ví dụ với UART ngắt nhận, các giao thức còn lại cũng sẽ có cách cấu hình tương tự.
### 8.3.1. Cấu hình ngắt UART
- Đầu tiên, các cấu hình tham số cho UART thực hiện bình thường.
- Trước khi cho phép UART hoạt động, cần kích hoạt ngắt UART bằng cách gọi hàm **USART_ITConfig();**
- Hàm **USART_ClearFlag(USART1, USART_IT_RXNE);** được gọi để xóa cờ ngắt ban đầu
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
### 8.3.2. Cấu hình NVIC
Ở NVIC, ta cấu hình tương tự như ngắt ngoài EXTI, ngắt Timer, tuy nhiên **NVIC_IRQChannel** được đổi thành **UARTx_IRQn** để khớp với line ngắt Timer.
```
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
```
### 8.3.3. Hàm phục vụ ngắt UART
- Hàm **USARTx_IRQHandler()** sẽ được gọi nếu xảy ra ngắt trên Line ngắt UART đã cấu hình. 
- Hàm **USART_GetITStatus** kiểm tra các cờ ngắt UART. Hàm này nhận 2 tham số là bộ USART và cờ tương ứng cần kiểm tra:
	- USART_IT_RXNE: Cờ ngắt nhận, cờ này set lên 1 khi bộ USART phát hiện data truyền tới.
	- USART_IT_TXE: Cờ ngắt truyền, cờ này set lên 1 khi USART truyền data đi.
- Có thể xóa cờ ngắt, gọi hàm **USART_ClearITPendingBit** để đảm bảo không còn ngắt trên line (thông thường cờ ngắt sẽ tự động xóa).
Trong hàm ngắt, ta thực hiện:
- Kiểm tra cờ ngắt từ bộ USART nào?
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




























