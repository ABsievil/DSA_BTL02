import random
import string

# Tạo danh sách các loại chuỗi
categories = ["LAPSE", "HAND", "KEITEIKEN", "KOKUSEN", "LIMITLESS", "CLEAVE"]

# Tạo chuỗi ngẫu nhiên với độ dài 30 kí tự
def generate_random_string():
    return ''.join(random.choices(string.ascii_letters, k= random.randint(1, 200) ))

# Số lượng chuỗi cần tạo
num_strings = 10000

# Xử lý và ghi vào file
with open("output_random.txt", "w") as file:
    print("Write file in output_random.txt")
    file.write(f"MAXSIZE {random.randint(1, 10)}\n")
    last_category = None
    for _ in range(num_strings):
        available_categories = [c for c in categories if c != last_category]
        category = random.choice(available_categories)
        if category == "HAND":
            #file.write(f"{category}\n")
            continue
        elif category == "KEITEIKEN":
            file.write(f"{category} {random.randint(1, 10)}\n")
        elif category == "KOKUSEN":
            file.write(f"{category}\n")
        elif category == "LIMITLESS":
            file.write(f"{category} {random.randint(1, 10)}\n")
        elif category == "CLEAVE":
            file.write(f"{category} {random.randint(1, 10)}\n")
        else:
            file.write(f"{category} {generate_random_string()}\n")
        
        last_category = category
    
    file.flush()  # Đảm bảo rằng tất cả dữ liệu đã được đẩy vào file
    file.close()  # Đóng file

print("Process finished !")
