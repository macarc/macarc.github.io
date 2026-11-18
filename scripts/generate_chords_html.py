import re
from pathlib import Path


def path_to_svg(path: Path):
    """Read an SVG file and return the HTML contents."""

    with open(path, "r") as f:
        contents = f.read()
    return f'<div class="chord">{contents}</div>'


def main():
    with open("chords.html", "r") as f:
        text = f.read()

    matches = re.findall(r"%([^<]*)", text)
    for match in matches:
        print(match)
        files_to_insert = sorted(Path.glob(Path(), f"../images/chords/{match}_*.svg"))
        text_to_insert = "\n".join(map(path_to_svg, files_to_insert))

        text = text.replace(f"%{match}", text_to_insert)

    with open("../chords.html", "w") as f:
        f.write(text)


if __name__ == "__main__":
    main()
