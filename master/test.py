import time


counter = 1
while True:
    print(counter)
    counter += 1
    time.sleep(0.5)
    if counter > 100:
        break
