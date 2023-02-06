示例目的：
          演示GTIM1和GTIM2联合产生IrDA的输出波形

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，PLL 8倍频至64MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=64MHz
          4. GTIM1生成38kHz载波
          5. GTIM2使用PWM模式调制GTIM1的38KHz载波。

演示说明：
         PB09输出IrDA调制的波形。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
