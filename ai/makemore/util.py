import pandas as pd

FILEPATH = "baby-names.csv"
YEAR = 2008
CHARS = ".abcdefghijklmnopqrstuvwxyz"
PAD = "."
N_CHARS = len(CHARS)

ctoi = {c: i for i, c in enumerate(CHARS)}
itoc = {i: c for i, c in enumerate(CHARS)}


def _remove_duplicates(items: list) -> list:
    seen = set()
    result = []
    for item in items:
        if item not in seen:
            seen.add(item)
            result.append(item)
    return result


def _get_names(csv_file_path, year):
    # Load the CSV file into a DataFrame
    df = pd.read_csv(csv_file_path)

    # Filter the DataFrame for the specified year
    # df = df[df["year"] == year]

    # Extract the 'name' column as a list
    names: list[str] = df["name"].to_list()

    names = _remove_duplicates(names)

    return [name.lower() for name in names]


def get_names():
    names = _get_names(FILEPATH, YEAR)
    print(f"name count: {len(names)}")
    return names
