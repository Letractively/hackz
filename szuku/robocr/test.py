from os import popen
from sys import stdin, stdout
from urllib import urlopen


while (True):
    image = open('captcha.jpeg', 'w')
    image.write(urlopen('http://szuku.pl/jcaptcha/jpeg/imageCaptcha').read())
    image.close()

    data = open('test.data', 'a')
    ocr = popen('./ocr.sh captcha').readline()[:-1]
    stdout.write('przeczytaj obrazek i wpisz napis:\n')
    human = stdin.readline()[:-1]

    if (human == 'koniec'):
        exit(0)

    data.write('%s %s\n' % (ocr, human))

data.close()

