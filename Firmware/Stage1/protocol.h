#ifndef __CART_PROTOCOL_H__
#define __CART_PROTOCOL_H__

/* messages */
#define MSG_REPLY		0
#define MSG_GET_VER		1
#define MSG_ENABLE_SERVO	2
#define MSG_SET_SERVO		3


/* reply codes */
#define REPLY_OK		0
#define REPLY_INVAL		1
#define REPLY_BUSY		2


struct control_msg {
    int8 kind;                  /* See MSG_XXXX defines */
    union {
        int8 reply;

        /* MSG_ENABLE_SERVO */
        struct {
            int8 ch1, ch2;
        } enable_servo;

        /* MSG_SET_SERVO */
        struct {
            int8 channel;
            int16 value;        /* in 10*us */
        } set_servo;
    } u;
};


struct reply_msg {
    int8 kind;                  /* MSG_XXX from input message */
    union {
        int8 reply;
        
        /* MSG_GET_VER */
        struct {
            int8 major, minor;
        } get_ver;
    } u;
};

#endif /* __CART_PROTOCOL_H__ */
