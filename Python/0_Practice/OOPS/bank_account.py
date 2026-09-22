class BankAccount:
    def __init__(self, owner_name, balance):
        self.owner_name = owner_name
        self.balance = balance
    
    def deposit(self, amount):
        self.balance += amount
        print(f"Amount Deposited: {amount}")
    
    def withdraw(self, amount):
        if amount > self.balance:
            print("Insufficient Balance")
        else:
            self.balance -= amount
            print(f"Amount Withdrawn: {amount}")
    
    def display(self):
        print(f"Owner Name: {self.owner_name}")
        print(f"Balance: {self.balance}")

