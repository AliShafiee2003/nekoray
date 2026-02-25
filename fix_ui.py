import sys

path = r'c:\Users\alish\Desktop\nekoray\nekoray-3.26\ui\mainwindow.ui'
with open(path, 'r', encoding='utf-8') as f:
    c = f.read()

c = c.replace('<enum>Qt::ToolButtonTextUnderIcon</enum>', '<enum>Qt::ToolButtonTextBesideIcon</enum>')

with open(path, 'w', encoding='utf-8') as f:
    f.write(c)

print("UI fixed successfully")
