"""Playground for testing transition formulas for animations.

Formulas retrieved from http://www.gizma.com/easing/
"""
import math
import itertools
from bokeh.io import output_file, show
from bokeh.models import Legend
from bokeh.plotting import figure
from bokeh.palettes import Category10

END_FRAME=50
START_VAL=0
END_VAL=100


def linear_tween(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate linearly.

    This code is commented to explain the structure of the transform functions.
    """
    # Calculate frame time coefficient.  This value allows us to calculate the
    # appopriate offset value between the start and end keyframes based on what.
    # frame we're currently at.
    t = (float(frame)/time)

    # The below math will yeild values between 0 and `end`, but we want the
    # function to output values between `start` and `end`.  Subtract `start`
    # from `end` so that we can get the correct value to offset `start` by
    # to produce the desired range.
    end -= start

    # Calculate the offset.  This is the primary math behind the ease function.
    offset = end * t + start

    # Return the result for this frame
    return start + offset

def quadratic_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * t**2 + start
    return start + offset

def quadratic_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    end -= start
    offset = -end * t*(t-2) + start
    return start + offset

def quadratic_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/(time*0.5)
    end -= start

    if t < 1:
        offset = (end*0.5) * t**2 + start
    else:
        t -= 1;
        offset = -(end*0.5) * (t*(t-2) - 1) + start

    return start + offset

def cubic_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * t**3 + start
    return start + offset

def cubic_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    t -= 1;
    end -= start
    offset = end * (t**3 + 1) + start
    return start + offset

def cubic_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/(time*0.5)
    end -= start

    if t < 1:
        offset = (end*0.5) * t**3 + start
    else:
        t -= 2;
        offset = (end*0.5) * (t**3 + 2) + start

    return start + offset

def quartic_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * t**4 + start
    return start + offset

def quartic_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    t -= 1;
    end -= start
    offset = -end * (t**4 - 1) + start
    return start + offset

def quartic_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/(time*0.5)
    end -= start

    if t < 1:
        offset = (end*0.5) * t**4 + start
    else:
        t -= 2;
        offset = -(end*0.5) * (t**4 - 2) + start

    return start + offset

def quintic_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * t**5 + start
    return start + offset

def quintic_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    t -= 1;
    end -= start
    offset = end * (t**5 + 1) + start
    return start + offset

def quintic_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/(time*0.5)
    end -= start

    if t < 1:
        offset = (end*0.5) * t**5 + start
    else:
        t -= 2;
        offset = (end*0.5) * (t**5 + 2) + start

    return start + offset

def sinusoidal_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = -end * math.cos(t * (math.pi/2)) + end + start
    return start + offset

def sinusoidal_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * math.sin(t * (math.pi/2)) + start
    return start + offset

def sinusoidal_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/time
    end -= start
    offset = -(end*0.5) * math.cos((t*math.pi) - 1) + start
    return start + offset

def exponential_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * 2**(10 * (t - 1)) + start
    return start + offset

def exponential_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    end -= start
    offset = end * (-(2**(-10 * t)) + 1) + start
    return start + offset

def exponential_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/(time*0.5)
    end -= start

    if t < 1:
        offset = (end*0.5) * 2**(10 * (t - 1)) + start
    else:
        t -= 1;
        offset = (end*0.5) * (-(2**(-10 * t)) + 2) + start

    return start + offset

def circular_ease_in(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate from zero velocity"""
    t = float(frame)/time
    end -= start
    offset = -end * (math.sqrt(1 - t**2) - 1) + start
    return start + offset

def circular_ease_out(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Decelerate to zero velocity"""
    t = float(frame)/time
    t -= 1;
    end -= start
    offset = end * math.sqrt(1 - t**2) + start
    return start + offset

def circular_ease_inout(frame, start=START_VAL, end=END_VAL, time=END_FRAME):
    """Accelerate until halfway, then decelerate"""
    t = float(frame)/(time*0.5)
    end -= start

    if t < 1:
        offset = -(end*0.5) * (math.sqrt(1 - t**2) - 1) + start
    else:
        t -= 2;
        offset = (end*0.5) * (math.sqrt(1 - t**2) + 1) + start

    return start + offset


def visualize_algorithms():
    p = figure(sizing_mode='stretch_both', title="Animation keyframe transition curves")
    frames = [i for i in range(0, END_FRAME+1)]
    def gen_data(func):
        return [func(frame) for frame in frames]

    def gen_color():
        for c in  itertools.cycle(Category10[10]):
            yield c
    color = gen_color()

    p_linear_tween = p.line(frames, gen_data(linear_tween), color=next(color))
    p_quadratic_ease_in = p.line(frames, gen_data(quadratic_ease_in), color=next(color))
    p_quadratic_ease_out = p.line(frames, gen_data(quadratic_ease_out), color=next(color))
    p_quadratic_ease_inout = p.line(frames, gen_data(quadratic_ease_inout), color=next(color))
    p_cubic_ease_in = p.line(frames, gen_data(cubic_ease_in), color=next(color))
    p_cubic_ease_out = p.line(frames, gen_data(cubic_ease_out), color=next(color))
    p_cubic_ease_inout = p.line(frames, gen_data(cubic_ease_inout), color=next(color))
    p_quartic_ease_in = p.line(frames, gen_data(quartic_ease_in), color=next(color))
    p_quartic_ease_out = p.line(frames, gen_data(quartic_ease_out), color=next(color))
    p_quartic_ease_inout = p.line(frames, gen_data(quartic_ease_inout), color=next(color))
    p_quintic_ease_in = p.line(frames, gen_data(quintic_ease_in), color=next(color))
    p_quintic_ease_out = p.line(frames, gen_data(quintic_ease_out), color=next(color))
    p_quintic_ease_inout = p.line(frames, gen_data(quintic_ease_inout), color=next(color))
    p_sinusoidal_ease_in = p.line(frames, gen_data(sinusoidal_ease_in), color=next(color))
    p_sinusoidal_ease_out = p.line(frames, gen_data(sinusoidal_ease_out), color=next(color))
    p_sinusoidal_ease_inout = p.line(frames, gen_data(sinusoidal_ease_inout), color=next(color))
    p_exponential_ease_in = p.line(frames, gen_data(exponential_ease_in), color=next(color))
    p_exponential_ease_out = p.line(frames, gen_data(exponential_ease_out), color=next(color))
    p_exponential_ease_inout = p.line(frames, gen_data(exponential_ease_inout), color=next(color))
    p_circular_ease_in = p.line(frames, gen_data(circular_ease_in), color=next(color))
    p_circular_ease_out = p.line(frames, gen_data(circular_ease_out), color=next(color))
    p_circular_ease_inout = p.line(frames, gen_data(circular_ease_inout), color=next(color))

    legend = Legend(items=[
        ('linear_tween', [p_linear_tween]),
        ('quadratic_ease_in', [p_quadratic_ease_in]),
        ('quadratic_ease_out', [p_quadratic_ease_out]),
        ('quadratic_ease_inout', [p_quadratic_ease_inout]),
        ('cubic_ease_in', [p_cubic_ease_in]),
        ('cubic_ease_out', [p_cubic_ease_out]),
        ('cubic_ease_inout', [p_cubic_ease_inout]),
        ('quartic_ease_in', [p_quartic_ease_in]),
        ('quartic_ease_out', [p_quartic_ease_out]),
        ('quartic_ease_inout', [p_quartic_ease_inout]),
        ('quintic_ease_in', [p_quintic_ease_in]),
        ('quintic_ease_out', [p_quintic_ease_out]),
        ('quintic_ease_inout', [p_quintic_ease_inout]),
        ('sinusoidal_ease_in', [p_sinusoidal_ease_in]),
        ('sinusoidal_ease_out', [p_sinusoidal_ease_out]),
        ('sinusoidal_ease_inout', [p_sinusoidal_ease_inout]),
        ('exponential_ease_in', [p_exponential_ease_in]),
        ('exponential_ease_out', [p_exponential_ease_out]),
        ('exponential_ease_inout', [p_exponential_ease_inout]),
        ('circular_ease_in', [p_circular_ease_in]),
        ('circular_ease_out', [p_circular_ease_out]),
        ('circular_ease_inout', [p_circular_ease_inout])
    ], location=(0,0))

    p.add_layout(legend, 'right')
    show(p)

visualize_algorithms()
