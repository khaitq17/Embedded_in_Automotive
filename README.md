# Embedded-Automotive

## Bài 2: GPIO

### 1. Cấp xung clock cho GPIO
Module RCC (Reset and Clock Control) cung cấp các hàm để cấu hình xung clock cho ngoại vi qua các Bus tương ứng.
![Picture1](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/daacd745-60e6-4499-bb41-86c84afb9edd)
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
#### Các đầu nối:
- **SCK** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- **MISO** (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- **MOSI** (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave.
- **SS** (Slave Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low).
  
![Picture2](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/59a2f364-6d36-4043-927f-1324d5c19944)
#### Quá trình truyền nhận dữ liệu:
- Master kéo chân SS của chân Slave muốn giao tiếp xuống mức 0 để báo hiệu muốn truyền nhận.
- Master cấp xung clock, với mỗi xung clock, 1 bit sẽ được truyền từ Master đến Slave và ngược lại.
- Các thanh ghi cập nhật giá trị và dịch 1 bit.
- Lặp lại quá trình đến khi truyền xong 8 bit thanh ghi.
  
![Picture3](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/f984fc6c-1364-4c0d-8566-e4bdb93302db)
#### Các chế độ hoạt động:
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
#### Các đầu nối:
- **SDA** (Serial Data): Đường truyền cho Master và Slave để gửi và nhận dữ liệu.
- **SCL** (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
      
![Picture4](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/89a016e7-34fa-443d-8736-0102a2a6a62a)
#### Quá trình truyền nhận dữ liệu:
- Start: Điều kiện: Chân SDA xuống mức 0 trước chân SCL.
- Truyền các bit địa chỉ để tìm Slave muốn giao tiếp.
- Bit R/W: Master gửi dữ liệu đi ứng với bit '0', nhận dữ liệu ứng với bit '1'.
- ACK: Chờ phản hồi, '0' là nhận và '1' là không nhận. Nếu không có Slave nào phản hồi, dùng Timer để thoát ra.
- Sau khi chọn được Slave để giao tiếp, bắt đầu truyền các bit dữ liệu đến Slave.
- Tương tự cũng có ACK để chờ phản hồi.
- Stop: Điều kiện: Chân SDA lên mức 1 trước chân SCL
   
 ![Picture5](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/d5ba043a-1d63-4de2-a7a6-bdb783e4acee)

### 3. UART - Universal Asynchronous Receiver - Transmitter
- Là chuẩn giao tiếp **KHÔNG** đồng bộ.
- Hoạt động ở chế độ song công.
- Dùng Timer nội để phân biệt 2 bit liền kề.
- Tốc độ truyền: Baudrate = Số bit truyền/1s
#### Các đầu nối:
- TX: Truyền.
- RX: Nhận.
      
![Picture6](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/9a52fd6e-eb76-43f4-aa81-9cce344ae3a7)
#### Quá trình truyền nhận dữ liệu:
- Start: 1 bit.
- Bit dữ liệu: 5 đến 9 bit.
- Bit chẵn lẻ:
	- Quy luật chẵn: Thêm một bit '0' hoặc '1' để số bit '1' là số chẵn.
	- Quy luật lẻ: Thêm một bit '0' hoặc '1' để số bit '1' là số lẻ.
- Stop: 1 đến 2 bit.
      
![Picture7](https://github.com/khaitq17/Embedded-Automotive/assets/159031971/56943f89-3f0a-4785-b944-51966027ff2b)

## BÀI 5: Giao tiếp SPI
### 1. SPI Software
SPI Software là cách “mô phỏng” bằng việc tạo ra một giao thức truyền thông giống SPI nhưng chỉ sử dụng GPIO của vi điều khiển.
#### Cấu hình GPIO cho SPI Software
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
#### Khởi tạo các chân cho SPI
```
void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}
```
#### Hàm truyền
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
#### Hàm nhận
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

### SPI Hardware
Trên mỗi dòng vi điều khiển khác nhau module SPI sẽ được tích hợp, điều khiển bởi các thanh ghi, phần cứng, IO khác nhau gọi là SPI cứng (SPI Hardware). STM32F1 có 2 khối SPI: SPI1 ở APB2 và SPI2 ở PAB1.
#### Cấu hình GPIO cho SPI Hardware
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
#### Cấu hình SPI
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
#### Các hàm truyền nhận
- Hàm SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data), tùy vào cấu hình datasize là 8 hay 16 bit sẽ truyền đi 8 hoặc 16 bit dữ liệu. Hàm nhận 2 tham số là bộ SPI sử dụng và data cần truyền.
- Hàm SPI_I2S_ReceiveData(SPI_TypeDef* SPIx) trả về giá trị đọc được trên SPIx. Hàm trả về 8 hoặc 16 bit data.
- Hàm SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG) trả về giá trị 1 cờ trong thanh ghi của SPI. Các cờ thường được dùng:
	- SPI_I2S_FLAG_TXE: Cờ báo truyền, cờ này sẽ set lên 1 khi truyền xong data trong buffer.
	- SPI_I2S_FLAG_RXNE: Cờ báo nhận, cờ này set lên 1 khi nhận xong data.
	- SPI_I2S_FLAG_BSY: Cờ báo bận,set lên 1 khi SPI đang bận truyền nhận.

Hàm truyền:
```
void SPI_Send1Byte(uint8_t data){
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);
   
    SPI_I2S_SendData(SPI1, data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0);
   
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);
}
```
Hàm nhận:
```
uint8_t SPI_Receive1Byte(void){
    uint8_t temp;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==1);
    temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==0);
    return temp;
}
```





















