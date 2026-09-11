import os

import lifecycle_msgs
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    EmitEvent,
    RegisterEventHandler,
)
from launch.events import matches_action
from launch.substitutions import (
    LaunchConfiguration,
)
from launch_ros.actions import LifecycleNode, Node
from launch_ros.event_handlers import OnStateTransition
from launch_ros.events.lifecycle import ChangeState


def generate_launch_description():
    log_level_arg = DeclareLaunchArgument(
        'log-level',
        default_value='info',
        description='Logging level',
    )
    log_level = LaunchConfiguration('log-level', default='info')

    config = os.path.join(
        get_package_share_directory('coresense_understanding_system_bringup'),
        'config',
        'kb_params.yaml',
    )

    triplestar_core_node = LifecycleNode(
        package='triplestar_core',
        executable='kb_node',
        name='triplestar_core',
        namespace='',
        output='screen',
        parameters=[config],
        arguments=['--ros-args', '--log-level', ['triplestar_kb:=', log_level]],
        emulate_tty=True,
    )

    triplestar_core_node_config_event = EmitEvent(
        event=ChangeState(
            lifecycle_node_matcher=matches_action(triplestar_core_node),
            transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CONFIGURE,  # type: ignore
        )
    )

    triplestar_core_node_activate_event = EmitEvent(
        event=ChangeState(
            lifecycle_node_matcher=matches_action(triplestar_core_node),
            transition_id=lifecycle_msgs.msg.Transition.TRANSITION_ACTIVATE,  # type: ignore
        )
    )

    marker_publisher_node = Node(
        package='triplestar_core',
        executable='kb_marker_publisher',
        name='marker_publisher',
        namespace='',
        output='screen',
    )

    viz_node = Node(
        package='triplestar_viz',
        executable='kb_visualizer_node',
        name='visualizer_node',
        namespace='triplestar_core',
        parameters=[
            {
                'store_path': '/tmp/triplestar_core'  # Add this line
            }
        ],
        output='screen',
    )

    _start_marker_publisher = RegisterEventHandler(
        OnStateTransition(
            target_lifecycle_node=triplestar_core_node,
            goal_state='active',
            entities=[marker_publisher_node],
        )
    )

    coresense_vampire_node = Node(
        package='coresense_vampire',
        executable='mynode.py',
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
    bt_executor_node = Node(
        package='krr_btcpp_ros2',
        executable='bt_executor',
        ros_arguments=['--params-file', 'src/decision_system/krr_btcpp_ros2/config/bt_executor.yaml'],
        name='bt_executor',
        namespace='',
        output='screen',
    )
    assemble_decision_node = Node(
        package='heuristic_assembly',
        executable='assemble_decision_heuristic_action_server',
        name='assemble_decision_heuristic',
        namespace='',
        output='screen',
    )
    adapt_decision_node = Node(
        package='heuristic_assembly',
        executable='adapt_decision_components_action_server',
        name='adapt_decision_components',
        namespace='',
        output='screen',
    )
    update_alternatives_node = Node(
        package='abstract_decision_components',
        executable='update_alternatives_prolog_action_server',
        name='update_alternatives_prolog',
        namespace='',
        output='screen',
    )
    update_cues_node = Node(
        package='abstract_decision_components',
        executable='update_cues_prolog_action_server',
        name='update_cues_prolog',
        namespace='',
        output='screen',
    )
    assess_node = Node(
        package='abstract_decision_components',
        executable='assess_action_server',
        name='assess',
        namespace='',
        output='screen',
    )
    aggregate_preferences_node = Node(
        package='abstract_decision_components',
        executable='aggregate_preferences_action_server',
        name='aggregate_preferences',
        namespace='',
        output='screen',
    )
    aggregate_utility_signed_node = Node(
        package='abstract_decision_components',
        executable='aggregate_utility_signed_action_server',
        name='aggregate_utility_signed',
        namespace='',
        output='screen',
    )
    order_dominating_node = Node(
        package='abstract_decision_components',
        executable='order_dominating_action_server',
        name='order_dominating',
        namespace='',
        output='screen',
    )
    order_lexicographical_node = Node(
        package='abstract_decision_components',
        executable='order_lexicographical_action_server',
     name='order_lexicographical',
        namespace='',
        output='screen',
    )
    order_condorcet_node = Node(
        package='abstract_decision_components',
        executable='order_condorcet_extension_action_server',
        name='order_condorcet',
        namespace='',
        output='screen',
    )
    take_best_node = Node(
        package='abstract_decision_components',
        executable='take_best_action_server',
        name='take_best',
     namespace='',
        output='screen',
    )
    eliminate_worst_node = Node(
        package='abstract_decision_components',
        executable='eliminate_worst_action_server',
        name='eliminate_worst',
        namespace='',
        output='screen',
    )
    accept_size_node = Node(
        package='abstract_decision_components',
        executable='accept_size_action_server',
        name='accept_size',
        namespace='',
        output='screen',
    )
    accept_satisficing_node = Node(
        package='abstract_decision_components',
        executable='accept_satisficing_action_server',
        name='accept_satisficing',
        namespace='',
        output='screen',
    )
    accept_always_node = Node(
        package='abstract_decision_components',
        executable='accept_always_action_server',
        name='accept_always',
        namespace='',
        output='screen',
    )
    prolog_node = Node(
        package='prolog_kb',
        executable='prolog_server',
        ros_arguments=['-p', 'knowledge_base:=both_test.pl'],
        name='prolog',
        namespace='',
        output='screen',
    )
    cue_visited_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=visited', '-p', 'query:=unlikely_if_visited(_A,V)'],
        name='cue_visited',
        namespace='',
        output='screen',
    )
    cue_distance_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=distance', '-p', 'query:=neg_distance(_A,V)'],
        name='cue_distance',
        namespace='',
        output='screen',
    )
    cue_habits_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=habits', '-p', 'query:=room_likely_from_habits(_A,V)'],
        name='cue_habits',
        namespace='',
        output='screen',
    )
    cue_doorway_status_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=doorway_status', '-p', 'query:=room_favorable_if_doorway_open(_A,V)'],
        name='cue_doorway_status',
        namespace='',
        output='screen',
    )
    cue_in_doorway_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=in_doorway', '-p', 'query:=object_unfavorable_if_in_doorway(_A,V)'],
        name='cue_in_doorway',
        namespace='',
        output='screen',
    )
    cue_is_book_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=is_book', '-p', 'query:=object_favorable_if_confidence_is_book(_A,V)'],
        name='cue_is_book',
        namespace='',
        output='screen',
    )
    cue_validity_node = Node(
        package='abstract_decision_components',
        executable='cue_prolog',
        ros_arguments=['-r', 'assess:=validity', '-p', 'query:=validity(_A,V)'],
        name='cue_validity',
        namespace='',
        output='screen',
    )
    return LaunchDescription(
        [
            log_level_arg,
            #assemble_decision_node, 
            #adapt_decision_node,
            update_alternatives_node,
            update_cues_node,
            assess_node,
            aggregate_preferences_node,
            aggregate_utility_signed_node,
            order_dominating_node,
            order_lexicographical_node,
            order_condorcet_node,
            take_best_node,
            eliminate_worst_node,
            accept_size_node,
            accept_satisficing_node,
            accept_always_node,
            prolog_node,
            cue_visited_node,
            cue_distance_node,
            cue_habits_node,
            cue_doorway_status_node,
            cue_in_doorway_node,
            cue_is_book_node,
            cue_validity_node,
            #triplestar_core_node,
            #triplestar_core_node_config_event,
            #triplestar_core_node_activate_event,
            #coresense_vampire_node,
            #bt_executor_node
            #coresense_understanding_node
            # start_marker_publisher,
            #viz_node,
        ]
    )
