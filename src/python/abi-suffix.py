import sysconfig
import platform
abi_tag = sysconfig.get_config_var('SOABI').replace('.', '') if sysconfig.get_config_var('SOABI') else None
if platform.system() == 'Windows':
    abi_tag += '.dll'
else:
    abi_tag += '.so'
abi_tag = '.' + abi_tag
print(abi_tag)