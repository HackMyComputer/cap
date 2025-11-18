from security import EncryptedPassword   

class User():
    encrypted_pass = EncryptedPassword('md5', None)

    def __init__(self):
        self.id = self.getLastUserId() + 1
        self.ip_addr = 

        self.configuration = {
            'username'  :f'{self.id}',
            'password'  :f'{self.encrypted_pass}'
            'ip_addr'   :f'{self.ip_addr}'
        }

    def createNewUser(): 
        with open()


    def getLastUserId()->int: 
        return 6005
