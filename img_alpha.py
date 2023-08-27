from PIL import Image
import os
def remove_background(image_path):
    # 打开图片
    image = Image.open(image_path)

    # 将图片转为带有 alpha 通道的模式
    image = image.convert("RGBA")

    # 获取图片的像素数据
    data = image.getdata()

    # 创建一个新的像素数据列表，将背景颜色设为透明
    new_data = []
    for item in data:
        # 判断当前像素是否为背景色（这里假设背景色为纯黑色）
        if item[:3] == (0, 0, 0):
            # 将背景色设为完全透明
            new_data.append((0, 0, 0, 0))
        else:
            # 非背景色保持不变
            new_data.append(item)

    # 更新图片的像素数据
    image.putdata(new_data)

    # 保存图片（保存为同名文件，覆盖原始图片）
    image.save(image_path)
def rm_bg_dir(dir_path):
    # 遍历输入文件夹中的所有文件
    for filename in os.listdir(dir_path):
        if filename.endswith('.png'):
            input_path = os.path.join(dir_path, filename)
            remove_background(input_path)
# 调用函数去除图片背景
# remove_background("./vector_icon_resized/crosshair.png")
rm_bg_dir(r"D:\Desktop\battlefield\vector_icon_resized_alpha")
