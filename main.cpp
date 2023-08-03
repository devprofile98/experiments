// #include <stdint.h>
#include <stdlib.h>
#include <iostream>

enum Test {
	MOSQ_ERR_SUCCESS,
	MOSQ_ERR_INVAL,
};

int mosquitto_topic_matches_sub(const char *sub, const char *topic, bool *result)
{
	size_t spos;

	if(!result) return MOSQ_ERR_INVAL;
	*result = false;

	if(!sub || !topic || sub[0] == 0 || topic[0] == 0){
		return MOSQ_ERR_INVAL;
	}

	if((sub[0] == '$' && topic[0] != '$')
			|| (topic[0] == '$' && sub[0] != '$')){

		return MOSQ_ERR_SUCCESS;
	}

	spos = 0;

	while(sub[0] != 0){
		if(topic[0] == '+' || topic[0] == '#'){
			return MOSQ_ERR_INVAL;
		}
		if(sub[0] != topic[0] || topic[0] == 0){ /* Check for wildcard matches */
			if(sub[0] == '+'){
				/* Check for bad "+foo" or "a/+foo" subscription */
				if(spos > 0 && sub[-1] != '/'){
					return MOSQ_ERR_INVAL;
				}
				/* Check for bad "foo+" or "foo+/a" subscription */
				if(sub[1] != 0 && sub[1] != '/'){
					return MOSQ_ERR_INVAL;
				}
				spos++;
				sub++;
				while(topic[0] != 0 && topic[0] != '/'){
					if(topic[0] == '+' || topic[0] == '#'){
						return MOSQ_ERR_INVAL;
					}
					topic++;
				}
				if(topic[0] == 0 && sub[0] == 0){
					*result = true;
					return MOSQ_ERR_SUCCESS;
				}
			}else if(sub[0] == '#'){
				/* Check for bad "foo#" subscription */
				if(spos > 0 && sub[-1] != '/'){
					return MOSQ_ERR_INVAL;
				}
				/* Check for # not the final character of the sub, e.g. "#foo" */
				if(sub[1] != 0){
					return MOSQ_ERR_INVAL;
				}else{
					while(topic[0] != 0){
						if(topic[0] == '+' || topic[0] == '#'){
							return MOSQ_ERR_INVAL;
						}
						topic++;
					}
					*result = true;
					return MOSQ_ERR_SUCCESS;
				}
			}else{
				/* Check for e.g. foo/bar matching foo/+/# */
				if(topic[0] == 0
						&& spos > 0
						&& sub[-1] == '+'
						&& sub[0] == '/'
						&& sub[1] == '#')
				{
					*result = true;
					return MOSQ_ERR_SUCCESS;
				}

				/* There is no match at this point, but is the sub invalid? */
				while(sub[0] != 0){
					if(sub[0] == '#' && sub[1] != 0){
						return MOSQ_ERR_INVAL;
					}
					spos++;
					sub++;
				}

				/* Valid input, but no match */
				return MOSQ_ERR_SUCCESS;
			}
		}else{
			/* sub[spos] == topic[tpos] */
			if(topic[1] == 0){
				/* Check for e.g. foo matching foo/# */
				if(sub[1] == '/'
						&& sub[2] == '#'
						&& sub[3] == 0){
					*result = true;
					return MOSQ_ERR_SUCCESS;
				}
			}
			spos++;
			sub++;
			topic++;
			if(sub[0] == 0 && topic[0] == 0){
				*result = true;
				return MOSQ_ERR_SUCCESS;
			}else if(topic[0] == 0 && sub[0] == '+' && sub[1] == 0){
				if(spos > 0 && sub[-1] != '/'){
					return MOSQ_ERR_INVAL;
				}
				spos++;
				sub++;
				*result = true;
				return MOSQ_ERR_SUCCESS;
			}
		}
	}
	if((topic[0] != 0 || sub[0] != 0)){
		*result = false;
	}
	while(topic[0] != 0){
		if(topic[0] == '+' || topic[0] == '#'){
			return MOSQ_ERR_INVAL;
		}
		topic++;
	}

	return MOSQ_ERR_SUCCESS;
}


int topicMatchesSub(const char *sub, const char *topic) {
  size_t spos;
  if (!sub || !topic || sub[0] == 0 || topic[0] == 0) {
    return false;
  }

  if ((sub[0] == '$' && topic[0] != '$') || (topic[0] == '$' && sub[0] != '$')) {

    return true;
  }

  spos = 0;

  while (sub[0] != 0) {
    if (topic[0] == '+' || topic[0] == '#') {
      return false;
    }
    if (sub[0] != topic[0] || topic[0] == 0) { /* Check for wildcard matches */
      if (sub[0] == '+') {
        /* Check for bad "+foo" or "a/+foo" subscription */
        if (spos > 0 && sub[-1] != '/') {
          return false;
        }
        /* Check for bad "foo+" or "foo+/a" subscription */
        if (sub[1] != 0 && sub[1] != '/') {
          return false;
        }
        spos++;
        sub++;
        while (topic[0] != 0 && topic[0] != '/') {
          if (topic[0] == '+' || topic[0] == '#') {
            return false;
          }
          topic++;
        }
        if (topic[0] == 0 && sub[0] == 0) {
          // *result = true;
          return true;
        }
      } else if (sub[0] == '#') {
        /* Check for bad "foo#" subscription */
        if (spos > 0 && sub[-1] != '/') {
          return false;
        }
        /* Check for # not the final character of the sub, e.g. "#foo" */
        if (sub[1] != 0) {
          return false;
        } else {
          while (topic[0] != 0) {
            if (topic[0] == '+' || topic[0] == '#') {
              return false;
            }
            topic++;
          }
          // *result = true;
          return true;
        }
      } else {
        /* Check for e.g. foo/bar matching foo/+/# */
        if (topic[0] == 0 && spos > 0 && sub[-1] == '+' && sub[0] == '/' && sub[1] == '#') {
          // *result = true;
          return true;
        }

        /* There is no match at this point, but is the sub invalid? */
        while (sub[0] != 0) {
          if (sub[0] == '#' && sub[1] != 0) {
            return false;
          }
          spos++;
          sub++;
        }

        /* Valid input, but no match */
        return true;
      }
    } else {
      /* sub[spos] == topic[tpos] */
      if (topic[1] == 0) {
        /* Check for e.g. foo matching foo/# */
        if (sub[1] == '/' && sub[2] == '#' && sub[3] == 0) {
          // *result = true;
          return true;
        }
      }
      spos++;
      sub++;
      topic++;
      if (sub[0] == 0 && topic[0] == 0) {
        // *result = true;
        return true;
      } else if (topic[0] == 0 && sub[0] == '+' && sub[1] == 0) {
        if (spos > 0 && sub[-1] != '/') {
          return false;
        }
        spos++;
        sub++;
        // *result = true;
        return true;
      }
    }
  }
  if ((topic[0] != 0 || sub[0] != 0)) {
    // *result = false;
  }
  while (topic[0] != 0) {
    if (topic[0] == '+' || topic[0] == '#') {
      return false;
    }
    topic++;
  }

  return true;
}



int main(){
	bool res;
	int ans = mosquitto_topic_matches_sub("gps/v2/+/+/+","gps/v2/cron/imei/3", &res);
	int ans2 = topicMatchesSub("gps/v2/+/imei/+","gps/v2/cron/imei/3");

	std::cout << res << "  " << ans << "  "<< ans2 <<std::endl;
	return 0;
}