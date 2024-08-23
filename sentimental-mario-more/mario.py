import cs50

# h = height
get_h = True

while get_h:
    h = cs50.get_int("Pyramid height: ")
    if h > 0 and h < 9:
        get_h = False
for i in range(h):
    print(" "*(h-i-1) + "#"*(i+1) + "  " + "#"*(i+1))
