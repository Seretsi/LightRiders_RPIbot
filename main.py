# starter bot refactored from Riddles Hackman python2 starter bot
import sys

from Bot.game import Game
from Bot.bot import Bot
import tensorflow as tf

def main():
    bot = Bot()
    game = Game()
    game.run(bot)
    hello = tf.constant('Hello, TensorFlow!')
    sess = tf.Session() 
    print(sess.run(hello))

if __name__ == '__main__':
    main()
