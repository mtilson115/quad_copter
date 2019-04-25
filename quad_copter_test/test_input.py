UP = '\x1b[A'
LEFT = '\x1b[D'
RIGHT = '\x1b[C'
DOWN = '\x1b[B'
while True:
    try:
        key = raw_input("Press an arrow")
        if key == UP:
            print "Up"
        if key == DOWN:
            print "Down"
        if key == RIGHT:
            print "Right"
        if key == LEFT:
            print "LEFT"
    except:
        print "Exiting"
        quit()
