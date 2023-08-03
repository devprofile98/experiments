
def does_match(key,received_topic):
    g_index = key.find("+")
    if g_index == -1:
        return False
    # breakpoint()
    if key[:g_index] == received_topic[:g_index]:
        new_key = key.find("/", g_index)
        new_t =received_topic.find("/", g_index)
        if new_t == -1 or new_key == -1:
            # breakpoint()
            if new_t != -1 or new_key != -1:
                return False
            return True
        # print(key)
        if does_match(key[new_key:], received_topic[new_t:]):
            return True
        return False
    else: 
        return False

# "gps/v2/cmd/imei/+", "gps/v2/cron/imei/+", 
# 
hashfunctinolist = [ "gps/v2/+/imei+/+", "gps/+/+/+/+/+","gps/v2/cron/imei/+" ]
wanted = "gps/v2/cron/imei/3"
print('----- :', wanted)
for i in hashfunctinolist:
    print(f"{does_match(i, wanted)} : {i}")








        # if t[g_index+1] "/":
    #     #     pass
    #     if t[:g_index] in received_topic:
    #         print("received topic matches the ", t)
