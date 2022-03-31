import os
import re
from datetime import datetime

# Path to the folder that has scripts inside:
path = os.curdir + '/../resources/'
# Path to scripting_layer.rc:
scripting_layer_resource_path = path + 'Gameplay.rc'
# Copy file content into file_content:
file_content = open(scripting_layer_resource_path, 'r').read()

# Search tokens:

# 1, 65535, 21177, 30570 => Limit values of the file version slots
# 1, dddyy, 0hhmm, 0ssmm => This is how we are going to set our version as

current = datetime.now()
additional = current.microsecond % 100

with_dots = current.strftime('1.%j%y.0%H%M.0%S')+str(additional)
with_commas = current.strftime('1,%j%y,0%H%M,0%S')+str(additional)

# Substitute file versions with the version we desire:
file_content = re.sub(re.escape('FILEVERSION ')+'(.*?)'+re.escape('\n'), 'FILEVERSION '+with_commas+'\n', file_content)
file_content = re.sub(re.escape('VALUE \"FileVersion\", \"')+'(.*?)'+re.escape('\"\n'), 'VALUE \"FileVersion\", \"'+with_dots+'\"\n', file_content)

open(scripting_layer_resource_path, 'w').write(file_content)