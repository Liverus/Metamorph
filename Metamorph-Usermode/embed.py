######################
##					##
##  File Embedder   ##
##					##
##	 This is BAD	##
##					##
######################

FILE_SOURCE = '../x64/Release/Metamorph-Kernel.sys'
FILE_DESTINATION = './driver.h'
VARIABLE_NAME = 'driver'

src = open(FILE_SOURCE, 'rb')

bytes_str = src.read().hex()
bytes_arr = '{' + (','.join(['0x'+bytes_str[i:i+2] for i in range(0, len(bytes_str), 2)])) + '}'

code = "auto {} = XOR({});".format(VARIABLE_NAME, bytes_arr)

src.close()

dest = open(FILE_DESTINATION, 'w')
dest.write(code)
dest.close()