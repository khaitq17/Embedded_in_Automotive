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




  


