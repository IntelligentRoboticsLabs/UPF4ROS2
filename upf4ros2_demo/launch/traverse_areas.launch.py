from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

    pkg_name = "upf4ros2_demo"

    #
    # ARGS
    #

    points = LaunchConfiguration("wps")
    declare_points_cmd = DeclareLaunchArgument(
        "wps",
        default_value=get_package_share_directory(
            pkg_name) + "/params/house.yaml")
        
    #
    # ACTIONS
    #

    upf4ros2_navigation_action_cmd = Node(
        package=pkg_name,
        executable="navigation_action_client",
        name="navigation_action_client",
        parameters=[points],
        output='screen')

    #
    # NODES
    #

    upf4ros2_demo_cmd = Node(
        package=pkg_name,
        executable="plan_executor",
        name="plan_executor",
        output='screen')

    ld = LaunchDescription()
    ld.add_action(declare_points_cmd)
    ld.add_action(upf4ros2_navigation_action_cmd)
    ld.add_action(upf4ros2_demo_cmd)

    return ld