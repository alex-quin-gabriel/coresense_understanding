import os

import lifecycle_msgs
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    IncludeLaunchDescription,
    DeclareLaunchArgument,
    EmitEvent,
    RegisterEventHandler,
)
from launch.events import matches_action
from launch.substitutions import (
    LaunchConfiguration,
    PathJoinSubstitution
)
from launch_ros.actions import LifecycleNode, Node
from launch_ros.event_handlers import OnStateTransition
from launch_ros.events.lifecycle import ChangeState
from launch_ros.substitutions import (
        FindPackageShare
)


def generate_launch_description():
    log_level_arg = DeclareLaunchArgument(
        'log-level',
        default_value='info',
        description='Logging level',
    )
    log_level = LaunchConfiguration('log-level', default='info')

    coresense_requirement_enforcer_node = Node(
        package='coresense_understanding_system',
        executable='requirement_enforcer_node',
        name='requirement_enforcer',
        namespace='',
        output='screen',
    )


    coresense_vampire_node = Node(
        package='coresense_vampire',
        executable='ros_vampire.py',
        name='coresense_vampire',
        namespace='',
        output='screen',
    )

    coresense_understanding_node = Node(
        package='coresense_understanding_system',
        executable='understanding_system_node',
        name='coresense_understanding_system',
        namespace='',
        output='screen',
    )

    bt_controller_node = Node(
        package='coresense_bt_controller',
        executable='bt_controller',
        parameters=[PathJoinSubstitution([
            FindPackageShare('coresense_understanding_system_bringup'), 'config', 'bt_controller.yaml'])
        ],
        name='bt_controller',
        namespace='',
        output='screen',
    )


    return LaunchDescription(
        [
            log_level_arg,
            IncludeLaunchDescription(
                PathJoinSubstitution([FindPackageShare('coresense_bringup'), 'launch', 'kb.launch.py']),
                launch_arguments={'bringup_package': 'coresense_understanding_system_bringup'}.items()
            ),
            bt_controller_node,
            coresense_requirement_enforcer_node,
            coresense_vampire_node,
            coresense_understanding_node
        ]
    )
