# A proof of concept implementation before moving to the arduino
import time
import random

SECRET = "password"
PIN = str(random.randint(1000, 9999))
MAX_BATTERY = 100

def start_puzzle():
  solved = False
  battery = MAX_BATTERY
  drain_battery = False

  while not solved:
    if battery <= 0:
      print("You were too slow, sorry.\nNo secret for you!")
      return
    guess = int(input("\nGuess: "))
    guess = round(guess, 4)
    guess = str(guess)
    
    timeout = time.time() + 3
    decrease_battery_rate = 0.01
    iteration = 0
    while True:
      iteration += 1

      # this ain't no timing sidechannel, so everything takes same time
      if time.time() > timeout:
        break

      display_calculating(solved, battery)
      if guess == PIN:
        solved = True
      elif drain_battery:
        battery -= (decrease_battery_rate / iteration)
        display_calculating(solved, battery)
      else:
        leading_nums_factor = 1
        for i in range(len(guess)):
          if guess[i] == PIN[i]:
            leading_nums_factor += 2
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
  start_puzzle()
  #display("", True, 80)
