#!/bin/python3
import time
import random
import argparse


def generate_random_hex(max_digits=100):
    """生成随机十六进制数（带0x前缀）"""
    digits = random.randint(1, max_digits)
    # 第一位不能是0
    first_digit = random.choice("123456789ABCDEF")
    other_digits = "".join(random.choice("0123456789ABCDEF") for _ in range(digits - 1))
    return "0x" + first_digit + other_digits


def hex_to_int(hex_str):
    """十六进制字符串转整数"""
    return int(hex_str, 16)


def int_to_hex(num):
    """整数转十六进制字符串（保留0x前缀，后面大写）"""
    if num < 0:
        return "-0x" + hex(-num)[2:].upper()
    else:
        return "0x" + hex(num)[2:].upper()


def generate_test(operator, number, max_digits, output_dir="/tmp"):
    with open(output_dir + f"/big_integer_test_{operator}", "w") as f:
        for _ in range(number):
            a_hex = generate_random_hex(max_digits)
            b_hex = generate_random_hex(max_digits)

            a_int = hex_to_int(a_hex)
            b_int = hex_to_int(b_hex)

            a_flag = random.choice([True, False])
            b_flag = random.choice([True, False])

            if a_flag:
                a_hex = "-" + a_hex
                a_int = -a_int
            if b_flag:
                b_hex = "-" + b_hex
                b_int = -b_int

            if operator == "add":
                c_int = a_int + b_int
            elif operator == "sub":
                c_int = a_int - b_int
            elif operator == "mul":
                c_int = a_int * b_int

            f.write("\n".join([a_hex, b_hex, int_to_hex(c_int)]) + "\n")
        f.flush()


def generate_div_test(number, max_digits, output_dir="/tmp"):
    with open(output_dir + f"/big_integer_test_div", "w") as f:
        for _ in range(number):
            a_hex = generate_random_hex(max_digits)
            b_hex = generate_random_hex(max_digits)

            a_int = hex_to_int(a_hex)
            b_int = hex_to_int(b_hex)

            c_int = a_int // b_int
            c_int_mod = a_int % b_int

            f.write(
                "\n".join([a_hex, b_hex, int_to_hex(c_int), int_to_hex(c_int_mod)])
                + "\n"
            )
        f.flush()


def generate_modpow_test(number, max_digits, output_dir="/tmp"):
    with open(output_dir + f"/big_integer_test_modpow", "w") as f:
        for _ in range(number):
            a_hex = generate_random_hex(max_digits)
            b_hex = generate_random_hex(max_digits)
            c_hex = generate_random_hex(max_digits)

            a_int = hex_to_int(a_hex)
            b_int = hex_to_int(b_hex)
            c_int = hex_to_int(c_hex)

            a_int %= c_int

            mp = pow(a_int, b_int, c_int)

            f.write("\n".join([a_hex, b_hex, c_hex, int_to_hex(mp)]) + "\n")
        f.flush()


def generate_rshift_test(number, max_digits, output_dir="/tmp"):
    with open(output_dir + f"/big_integer_test_rshift", "w") as f:
        for _ in range(number):
            a_hex = generate_random_hex(max_digits)
            b = random.randint(0, max_digits // 60)

            a_int = hex_to_int(a_hex)

            c_int = a_int // (2**b)

            f.write("\n".join([a_hex, str(b), int_to_hex(c_int)]) + "\n")
        f.flush()


def generate_bigcmp_test(number, max_digits, output_dir="/tmp"):
    with open(output_dir + f"/big_integer_test_bigcmp", "w") as f:
        for _ in range(number):
            a_hex = generate_random_hex(max_digits)
            b_hex = generate_random_hex(max_digits)

            a_int = hex_to_int(a_hex)
            b_int = hex_to_int(b_hex)

            a_flag = random.choice([True, False])
            b_flag = random.choice([True, False])

            if a_flag:
                a_hex = "-" + a_hex
                a_int = -a_int
            if b_flag:
                b_hex = "-" + b_hex
                b_int = -b_int

            c = f"{(a_int > b_int):d} {(a_int == b_int):d} {(a_int < b_int):d}"
            d = f"{(a_int > 0):d} {(a_int == 0):d} {(a_int < 0):d}"

            f.write("\n".join([a_hex, b_hex, c, d]) + "\n")
        f.flush()


def main():
    parser = argparse.ArgumentParser(description="生成大整数运算测试用例")
    parser.add_argument(
        "operation",
        choices=["add", "sub", "mul", "div", "rshift", "modpow", "bigcmp"],
        help="运算类型: add(加), sub(减), mul(乘), div(除), rshift(右移), modpow(指数模), bigcmp(比较)",
    )
    parser.add_argument(
        "-n", "--number", type=int, default=1, help="生成测试用例的数量 (默认: 1)"
    )
    parser.add_argument(
        "-m", "--max-digits", type=int, default=100, help="最大位数 (默认: 100)"
    )
    parser.add_argument(
        "-s", "--seed", type=int, default=0, help="随机数种子 (默认: 0表示time(0))"
    )

    args = parser.parse_args()

    # 设置随机种子以便重现结果
    random.seed(None if args.seed == 0 else args.seed)

    print(f"生成 {args.number} 个 {args.operation} 运算测试用例")

    if args.operation in ["add", "sub", "mul"]:
        generate_test(args.operation, args.number, args.max_digits)
    elif args.operation == "div":
        generate_div_test(args.number, args.max_digits)
    elif args.operation == "rshift":
        generate_rshift_test(args.number, args.max_digits)
    elif args.operation == "modpow":
        generate_modpow_test(args.number, args.max_digits)
    elif args.operation == "bigcmp":
        generate_bigcmp_test(args.number, args.max_digits)


if __name__ == "__main__":
    main()
