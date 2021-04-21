import numpy as np
from PIL import Image

colorList = [
    (0, 0, 0),
    (255, 255, 0),
    (0, 255, 255),
    (255, 0, 255),
    (255, 0, 0),
    (0, 255, 0),
    (0, 0, 255),
    (255, 255, 255),
]

id_color = {

}

def main():
    im = None
    pixels = None
    color_list_index = 0
    with open("./result.txt") as file:
        nonempty_lines = [line.strip("\n") for line in file if line != "\n"]
        for i in range(len(nonempty_lines) - 1):
            line = nonempty_lines[i].replace("\n", "").split("\t")
            for j in range(len(line) - 1):
                if not im:
                    im = Image.new("RGB", (len(nonempty_lines), len(line)), "#000000")
                    pixels = im.load()

                if line[j] not in id_color.keys():
                    if len(colorList) == color_list_index:
                        colorList.append(tuple(np.random.choice(range(256), size=3)))
                    id_color[line[j]] = colorList[color_list_index]
                    color_list_index += 1

                pixels[i, j] = id_color[line[j]]

    im.show()
    im.save("./image.jpg")


if __name__ == '__main__':
    main()
