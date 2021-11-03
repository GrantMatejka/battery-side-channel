# A proof of concept implementation before moving to the arduino
import time
import random
import sys

SECRET = "password"
PIN = "%s" % random.randint(1000, 9999)
MAX_BATTERY = 100

def start_puzzle():
  solved = False
  battery = MAX_BATTERY
  drain_battery = False

  # we will give it to them, where if they solve it while simultaneously draining
  # the battery, it'll be ok
  while not solved:
    if battery <= 0:
      print("\nYou were too slow, sorry.\nNo secret for you!")
      return
    guess = int(input("\nGuess: "))
    guess = round(guess, 4)
    guess = str(guess)
    
    timeout = time.time() + 2
    decrease_battery_rate = 0.000005
    while True:
      time_remaining = timeout - time.time()
      # this ain't no timing sidechannel, so everything takes same time
      if time_remaining <= 0:
        break

      display_calculating(solved, battery)
      if guess == PIN:
        solved = True

      if drain_battery:
        battery -= (decrease_battery_rate / (time_remaining + 1))
      else:
        leading_nums_factor = 1
        for i in range(len(guess)):
          if guess[i] == PIN[i]:
            leading_nums_factor *= 2
          else:
            break
        decrease_battery_rate *= leading_nums_factor
        drain_battery = True
    drain_battery = False

    display_result(solved, battery)


def display_calculating(solved, battery):
  msg = "{:30s}      {:3.2f}%".format("Calculating", battery)
  print(msg, end='\r')

def display_result(solved, battery):
  end_char = '\n' if solved else '\r'
  valid = "Correct" if solved else "Incorrect Credentials"
  msg = "\n{:30s}      {:3.2f}%".format(valid, battery)
  print(msg, end=end_char)


if __name__ == "__main__":
  if len(sys.argv) > 1 and sys.argv[1] == "-c":
      print(PIN)

  start_puzzle()
  #display("", True, 80)
