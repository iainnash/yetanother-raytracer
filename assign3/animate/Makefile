SCENES = $(wildcard scenes/*.scene)
SCENE_RENDER = $(addprefix jpgs/,$(notdir $(SCENES:.scene=.jpg)))
BUILDERS = $(wildcard builders/*)

PROGRAM = ../assign3

all: render

clean:
	rm jpgs/*.jpg
	rm scenes/*.scene

scene: $(BUILDERS)
	$(info rendered all builders)

.PHONY: builders/%

builders: $(BUILDERS)
	sh $<

jpgs/%.jpg: scenes/%.scene
	$(PROGRAM) $< $@ --nodisplay

render: $(SCENE_RENDER) $(BUILDERS)
	$(info Rendered all scene files)

gifs/balls.gif: jpgs/balls_scene*.jpg
	#animate -resize 50% -delay 2 -loop 1 jpgs/balls_scene*.jpg $@
	ffmpeg -i jpgs/balls_scene%03d.jpg $@

gifz: gifs/balls.gif
	$(info created gif)