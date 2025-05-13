# Rock Paper Scissor Game

import random

# list of available choices 
available_choices = ['rock','paper','scissors','quit']

# initialise user and computer's score to be 0.
user_score = 0
computer_score = 0

# a list of winning condition pair where 1st one is user's choice and 2nd one is computer's choice
win_conditions = [('rock','scissor'),('paper','rock'),('scissors','paper')]

def displayScore():
    # Utility function to display scores
    print(f"Your Score: {user_score}")
    print(f"Computer Score: {computer_score}")

# Prompts user to enter a valid choice i.e. 'rock'/'paper'/'scissor'/'quit'
user_choice = input("Enter Your Choice: ")

# continue till user enters "quit"
while user_choice.lower() != 'quit':
    # if user enters an invalid choice, show a message
    if(user_choice.lower() not in available_choices):
        print("Please Enter a valid choice: rock/paper/scissors/quit")
    else: 
        # generate random computer choice
        computer_choice = available_choices[random.choice([0,1,2])]

        # if same choice show a tie message
        if computer_choice == user_choice:
            print("It's a Tie")
            displayScore()
        else:
            # check for win condition
            condition = (user_choice,computer_choice)

            # if user wins then display a message
            if condition in win_conditions:
                print("You Win!!")
                user_score += 1
                displayScore()
            # if computer win display a message
            else:
                print("Computer Wins!")
                computer_score += 1
                displayScore()
    user_choice = input("Enter Your Choice: ")


        
    