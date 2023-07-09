from flask import Flask
from flask import request
from flask import jsonify
import base64
import sysv_ipc
import struct
import sys
import os
import time
import logging

ipc_key=123456

PORT = int(os.environ['PORT'])

OFFSET_OP            = 0
OFFSET_REQ           = 8
OFFSET_MEM_IDX       = 16
OFFSET_W_VAL         = 24
OFFSET_RES_R_VAL     = 32
OFFSET_CURR_REQ_TIME = 40
OFFSET_RES_ECU_TIME  = 48
OFFSET_RES           = 56

REQ_READ  = 1
REQ_WRITE = 2
REQ_TIME  = 4

CMD_SIZE = 8

LOGFILE = "app_server2.log"

app = Flask('app server')

app.logger.setLevel(logging.DEBUG)
fh = logging.FileHandler(LOGFILE)
fh.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
app.logger.addHandler(fh)


@app.route('/v1/<string:ecuname>', methods=["GET", "PUT"])
def variable_operation(ecuname):
    req      = request.args.get('req')
    req_time = float(request.args.get('req_time'))

    # Client Request set to Read Request
    if req == "read":
        # Create Shared Memory if no yet created
        memory = sysv_ipc.SharedMemory(ipc_key, sysv_ipc.IPC_CREAT, size=64)

        # Set Read Request Operation to Client
        a=(REQ_READ).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_REQ )

        # Set Memory Index to Client
        mem_idx  = int(request.args.get('mem_idx'))
        a=(mem_idx).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_MEM_IDX )

        # Set Write Value(0) to Client
        a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=True)
        memory.write( a, OFFSET_W_VAL )

        # Set Current Time to Client
        a=struct.pack('<d', req_time)
        memory.write( a, OFFSET_CURR_REQ_TIME )

        # Command to Client
        a=(1).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_OP )

        # Wait for Client Read Response
        while True:
            # Set Client Read Response(1byte) to res
            res = int.from_bytes(memory.read(1, OFFSET_RES), 'little',signed=True)
            # Client Read Response set to non NoResponse(0)
            if 0 != res:
                # Set Client Read Response to r_val
                r_val = int.from_bytes(memory.read(CMD_SIZE, OFFSET_RES_R_VAL), 'little')

                # Set ECU time to res_ecu_time
                res_ecu_time = struct.unpack('<d', memory.read(CMD_SIZE, OFFSET_RES_ECU_TIME ))[0]

                # Operation Request Clear
                a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=False)
                memory.write( a, OFFSET_OP )

                # Client Response Clear
                a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=True)
                memory.write( a, OFFSET_RES)

                # Client Response is finish normally
                if 1 == res:
                    # Set Client Response value to zero
                    res = 0
                break

        # debug
        app.logger.debug("server2.py: req=read " + "ecuname=" +str(ecuname) + " mem_idx=" +str(mem_idx) + " res_ecu_time=" +str(res_ecu_time) + " readres=" +str(r_val))

        return jsonify({ "ecuname" : ecuname, "mem_idx" : mem_idx, "res_ecu_time" : res_ecu_time, "readres" : str(r_val), "err" : str(res) })
    # Client Request set to Write Request
    elif req == 'write':
        # Create Shared Memory if no yet created
        memory = sysv_ipc.SharedMemory(ipc_key, sysv_ipc.IPC_CREAT, size=64)

        # Set Write Request Operation to Client
        a=(REQ_WRITE).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_REQ )

        # Set Memory Index to Client
        mem_idx  = int(request.args.get('mem_idx'))
        a=(mem_idx).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_MEM_IDX )

        # Set Write Value to Client
        w_val = int(request.args.get('w_val'))
        a=(w_val).to_bytes(CMD_SIZE, byteorder='little',signed=True)
        memory.write( a, OFFSET_W_VAL )

        # Set Current Time to Client
        a=struct.pack('<d', req_time)
        memory.write( a, OFFSET_CURR_REQ_TIME )

        # Command to Client
        a=(1).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_OP )

        # Wait for Client Write Response
        while True:
            # Set Client Write Response(1byte) to res
            res = int.from_bytes(memory.read(1, OFFSET_RES), 'little',signed=True)
            # Client Write Response set to non NoResponse(0)
            if 0 != res:
                # Set Client Read back Response to r_val
                r_val = int.from_bytes(memory.read(CMD_SIZE, OFFSET_RES_R_VAL), 'little',signed=True)

                # Set Client Response value to READ BACK ERROR if Read back Value != Write Value
                if w_val != r_val:
                    res = -2

                # Set ECU time to res_ecu_time
                res_ecu_time = struct.unpack('<d', memory.read(CMD_SIZE, OFFSET_RES_ECU_TIME ))[0]

                # Operation Request Clear
                a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=False)
                memory.write( a, OFFSET_OP )

                # Client Response Clear
                a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=True)
                memory.write( a, OFFSET_RES)

                # Client Response is finish normally
                if 1 == res:
                    # Set Client Response value to zero
                    res = 0
                break
 
        # debug
        app.logger.debug("server2.py: req=write " + "ecuname=" +str(ecuname) + " mem_idx=" +str(mem_idx) + " res_ecu_time=" +str(res_ecu_time) + " readback=" +str(r_val))

        return jsonify({ "ecuname" : ecuname, "mem_idx" : mem_idx, "res_ecu_time" : res_ecu_time, "err" : str(res), "readback" : str(r_val) })
    # Client Request set to Time Request
    elif req == "time":
        # Create Shared Memory if no yet created
        memory = sysv_ipc.SharedMemory(ipc_key, sysv_ipc.IPC_CREAT, size=64)

        # Set Read Request Operation to Client
        a=(REQ_TIME).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_REQ )

        # Set Memory Index(0) to Client
        a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_MEM_IDX )

        # Set Write Value(0) to Client
        a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=True)
        memory.write( a, OFFSET_W_VAL )

        # Set Current Time to Client
        a=struct.pack('<d', req_time)
        memory.write( a, OFFSET_CURR_REQ_TIME )

        # Command to Client
        a=(1).to_bytes(CMD_SIZE, byteorder='little',signed=False)
        memory.write( a, OFFSET_OP )

        # Wait for Client Time Response
        while True:
            # Set Client Read Response(1byte) to res
            res = int.from_bytes(memory.read(1, OFFSET_RES), 'little',signed=True)
            # Client Read Response set to non NoResponse(0)
            if 0 != res:
                # Set ECU time to res_ecu_time
                res_ecu_time = struct.unpack('<d', memory.read(CMD_SIZE, OFFSET_RES_ECU_TIME ))[0]

                # Operation Request Clear
                a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=False)
                memory.write( a, OFFSET_OP )

                # Client Response Clear
                a=(0).to_bytes(CMD_SIZE, byteorder='little',signed=True)
                memory.write( a, OFFSET_RES)

                # Client Response is finish normally
                if 1 == res:
                    # Set Client Response value to zero
                    res = 0
                break

        return jsonify({ "ecuname" : ecuname, "res_ecu_time" : res_ecu_time, "err" : str(res) })
    else:
        print("req out of range")

app.run(debug=True, host='0.0.0.0', port=PORT)
