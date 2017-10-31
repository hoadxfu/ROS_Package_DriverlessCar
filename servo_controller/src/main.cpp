#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "api_i2c_pwm.h"
PCA9685 *pca9685 = new PCA9685() ;
int dir = 0, throttle_val = 30;
char key = 0;
int getkey() {
    int character; 
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}
void chatterCallback(const std_msgs::Int32::ConstPtr& msg){
	double theta=msg->data;
	api_set_STEERING_control( pca9685,theta);
	api_set_FORWARD_control(pca9685, throttle_val);
	ROS_INFO("new theta: [%f]",theta);
	if(getkey()== 27)throttle_val = 0;
} 
int main(int argc, char **argv)
{
	ros::init(argc, argv, "servo_controller");
    api_pwm_pca9685_init( pca9685 );
    ros::NodeHandle n;
   ros::Subscriber sub = n.subscribe("center_point", 1000, chatterCallback);

  /**
   * Spin dùng để loop các message
   */
  ros::spin();
	
  return 0;

}

