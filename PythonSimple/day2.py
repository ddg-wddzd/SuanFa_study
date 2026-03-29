a=15
b=4
print(a+b) #加法 输出19
print(a-b) #减法 输出11
print(2.5*4) #乘法 输出10.0(浮点数与整数相乘结果位浮点数)
print(10/2) #除法 输出5.0(除法结果总是浮点数)
print(9/4) #除法 输出2.25
print(10//3) #整数除法 输出3(结果向下取整)
print(-10//3) #整数除法 输出-4(因为 -3.3333333,结果向下取整)
print(10%3) #取余 输出1(10除以3的余数)
print(-10%3) #取余 输出2(因为 -10除以3的商是-4，余数是2)
print(2**3) #幂运算 输出8(2的3次方)
print(9**0.5) #开方 输出3.0(9的平方根)
#   **的优先级高于*和/，所以2**3*4等价于(2**3)*4 输出32
# 优先级从高到低：()  >  **  >  * / // %  >  + - 
# 括号 > 幂 > 正负号 > 乘除取余 > 加减

#示例
result=2+3*4**2 #等价于2+3*(4**2) 输出50
print(result) 
result=(2+3)*4**2 #等价于(2+3)*(4**2) 输出80
print(result)
print(10+20//3*2) #等价于10+(20//3)*2 输出22
# 当优先级相同时，Python 从左到右依次计算（除了幂运算从右到左）
print(10/2*3) #等价于(10/2)*3 输出15.0
print(2**3**2) #等价于2**(3**2) 输出512

#综合示例
#计算圆的面积
radius=5
area=3.14*radius**2 #等价于3.14*(radius**2) 输出78.5
print("面积为：",area)

#判断一个数是否为奇数
num=7
if num%2==1:
    print(f"{num} 是奇数")

#求平均数
scores=[85,92,78,90]
average=sum(scores)/len(scores) #等价于(85+92+78+90)/4 输出86.25
print("平均数为：",average)


name=input("请输入你的名字：") #获取用户输入
print("你好,"+name)
print(f"欢迎,{name}!")

#由于 input() 返回的是字符串，如果我们需要数字（整数、浮点数）来进行数学计算，就必须进行显式类型转换。
#1.转换为整数int()
age_str=input("请输入你的年龄：")
age=int(age_str) #将输入的字符串转换为整数
print(f"明年你就{age+1}岁了")
#2.转换为浮点数float()
height_str=input("请输入你的身高（米）：")
height=float(height_str) #将输入的字符串转换为浮点数
print(f"你的身高是：{height}米")

#3.直接转换的简化写法
age=int(input("请输入年龄："))
height=float(input("请输入身高："))
print(f"年龄：{age},身高：{height}米")

#处理转换错误
#当用户输入的内容无法转换为数字时（例如输入“abc”作为年龄），程序会抛出 ValueError 并终止。为了让程序更健壮，我们可以使用 try...except 捕获异常。
while True:
    try:
        age=int(input("请输入您的年龄："))
        break #如果转换成功，跳出循环
    except ValueError:
        print("输入无效，请输入一个整数。")
print(f"你的年龄是：{age}")

