import os
from PIL import Image

input_folder = r"D:\Desktop\battlefield\vector_icon"  # 输入文件夹路径
output_folder = r"D:\Desktop\battlefield\vector_icon_resized" # 输出文件夹路径
output_size = (50, 50)  # 输出图片尺寸

# 确保输出文件夹存在
os.makedirs(output_folder, exist_ok=True)

# 遍历输入文件夹中的所有文件
for filename in os.listdir(input_folder):
    if filename.endswith('.png'):
        input_path = os.path.join(input_folder, filename)
        output_path = os.path.join(output_folder, filename)

        # 打开输入图片并进行尺寸调整
        image = Image.open(input_path)
        image = image.resize(output_size, Image.ANTIALIAS)

        # 保存压缩后的图片到输出文件夹
        image.save(output_path)

print('压缩完成')