New function : "transform piece" to change the piece into background (as gb only support a limited of sprite). Just debug Tetris with bgb and you'll see


It is still difficult because bloc is only considered as only 1 location, but not as a metasprite

Would be easier to code if the bloc was only 1 tile * tile

As a matter of fact, you can't yet :
- create a 2nd line
- colide correctly on left / right


Also on this alpha, canplayermove and canplayermove_down are the exact same function, so this can (very) easily be optimized
