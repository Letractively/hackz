#!/usr/bin/python
# coding=utf-8

from thread import start_new_thread
from os import popen
from time import sleep, time
from string import letters, digits
from re import compile, search, U
from random import randint, choice, sample, seed
from urllib import urlencode
from mechanize import Browser, CookieJar


def only_letters_or_digits(s):
    return filter(lambda x: x in letters or x in digits, s)

seed(time())

def rand_string():
    n = randint(3, 30)
    factor = n/len(letters) + 1
    return ''.join(sample(letters*factor, n))

imiona = open('imiona.txt').readlines()
nazwiska = open('nazwiska.txt').readlines()

def rand_login():
    imie = sample(imiona, 1)[0][:-1]
    nazwisko = sample(nazwiska, 1)[0][:-1]

    if (imie != 'Mateusz' or nazwisko != 'Wykurz'):
        return '%s.%s' % (imie, nazwisko)
    else:
        return rand_login()

maile = open('email.txt').readlines()

def rand_provider():
    return sample(maile, 1)[0][:-1]

def rand_mail():
    x = randint(1, 10)
    if (x == 1):
        return '%s@%s' % (rand_string(), rand_provider())
    elif (x == 2):
        return '%s@%s.com' % (rand_login(), rand_string())
    elif (x == 3):
        return '%s@%s.com' % (rand_string(), rand_string())
    else:
        return '%s@%s' % (rand_login(), rand_provider())

ok_regex = compile('Dziękujemy za zainteresowanie')

connections = 0
successful = 0
errors = 0

def go(thread_num):
    global connections
    global successful
    global errors

    file_name = str(thread_num)
    br = Browser()
    while(True):
        try:
            br.retrieve('http://szuku.pl/jcaptcha/jpeg/imageCaptcha', file_name + '.jpeg')
            
            ocr = popen('./convert.sh ' + file_name).read()
            captcha = only_letters_or_digits(ocr)

            br.open('http://szuku.pl/teaser/save', 
                    urlencode({'email': rand_mail(),
                               'captcha': captcha}))
            
            connections += 1

            if ok_regex.search(br.response().read()):
                successful += 1

        except Exception, e:
            print e
            errors += 1


for i in range(20):
    start_new_thread(go, (i,))

while(True):
    sleep(3)
    print 'Connections: %d\tsuccessful: %d\terrors: %d' % (connections, successful, errors)
 
