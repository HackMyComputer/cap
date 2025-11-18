import hashlib 

import asterisk_models

class EncryptedPassword(): 
    hexdigit = None 

    def __init__(self, encryption_used:str, data=None): 
        m = data.encode('utf-8') if data is not None else self.__default_conf()
 
        h = hashlib.new(encryption_used, usedforsecurity=True)
        h.update(m)
            
        self.hexdigest = h.hexdigest() 
        self.h = h

    def __default_conf(self): 
        id = asterisk_models.utils.getLastUserId() 
        return b'{id}pass'
        
