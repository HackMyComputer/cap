from asterisk_models.utils.security import EncryptedPassword 

_obj = EncryptedPassword('md5', 'JwefnfD903#')

print(_obj.hexdigest)
