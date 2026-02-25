import sys

path = r'c:\Users\alish\Desktop\nekoray\nekoray-3.26\ui\mainwindow.ui'
with open(path, 'r', encoding='utf-8') as f:
    c = f.read()

c = c.replace(':/neko/icon/menu.png', ':/neko/icon/menu_white.svg')
c = c.replace(':/neko/icon/search.png', ':/neko/icon/search_white.svg')

with open(path, 'w', encoding='utf-8') as f:
    f.write(c)

print("UI fixed successfully with SVG paths")
