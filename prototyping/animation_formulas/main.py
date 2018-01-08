ANIMATION_START=20
FRAME_END=500

def main()
    start_val = 5
    a = animation.Transition(loop=False);
    a.AddKeyframe(animation.seconds_to_frames(1.0), end_val, EASE_LINEAR)
    a.AddKeyframe(animation.seconds_to_frames(2.0), end_val, EASE_LINEAR)

    a.reset(start_val, ANIMATION_START)
    a.play()

    val = start_val
    for frame in range(0, FRAME_END+1):
       val = a.CalcMatrixTransform(frame, val)
