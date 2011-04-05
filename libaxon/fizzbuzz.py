def fizz_buzz():
    for i in range(1,101):
        print i,
        if not i % 3:
            print "Fizz",
        if not i % 5:
            print "Buzz",
        print

if __name__ == "__main__":
    fizz_buzz()
