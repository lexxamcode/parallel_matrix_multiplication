from os import*

if __name__ == '__main__':
    mkdir("../entire output")
    i = 50
    while i <= 1600:
        mkdir(f'../entire output/m{i}')
        i *= 2
