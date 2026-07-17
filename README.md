# life-saver

Backup tool for git repositories. Scans directories for `.git` folders and packages everything into a single `.tar.gz` archive — ideal for quick backups to physical devices or remote servers.

## Features

- Scan directories and auto-discover git repositories
- Include raw directories without git detection
- Combine multiple inputs in a single archive
- Auto-generated timestamped output filenames
- Available as source build or `.deb` package

## Installation

### From source

```bash
git clone https://github.com/Gabrielito666/life-saver.git
cd life-saver
make install
```

### Debian package

```bash
git clone https://github.com/Gabrielito666/life-saver.git
cd life-saver
make debian
sudo dpkg -i debian/life-saver_*.deb
```

### Uninstall

```bash
make uninstall
```

## Usage

```bash
life-saver [OPTIONS]
```

> At least one `-i` or `-r` is required.

## Options

| Flag | Long | Description | Required |
|------|------|-------------|----------|
| `-i` | `--input` | Directory to scan for git repos (repeatable) | Yes* |
| `-r` | `--raw-input` | Directory to include directly, no git detection (repeatable) | Yes* |
| `-o` | `--output` | Output `.tar.gz` file path | No |
| `-h` | `--help` | Show help message | No |

\* At least one `-i` or `-r` must be provided.

### Default output

When `-o` is not specified, the output file is auto-generated with the format:

```
life-saver-backup-YYYY-MM-DD-HHMMSS.tar.gz
```

## Examples

**Backup all git repos under a parent directory:**

```bash
life-saver -i ~/projects
```

**Backup multiple directories:**

```bash
life-saver -i ~/projects -i ~/work -o my-backup.tar.gz
```

**Mix git repos with raw config directories:**

```bash
life-saver -i ~/projects -r ~/.config/nvim -r ~/dotfiles
```

**Multiple raw inputs:**

```bash
life-saver -r ~/.ssh -r ~/.gnupg -r ~/docs -o configs-backup.tar.gz
```

## Development

### Lab environment

Build the `.deb`, create a Docker container with test fixtures, and experiment interactively:

```bash
make lab
```

### Run tests

```bash
make test
```

### Clean build artifacts

```bash
make clean
```

## Project Structure

```
life-saver/
├── include/           # Header files
│   ├── colors.h
│   ├── command_help.h
│   ├── command-saver.h
│   ├── list-items.h
│   ├── pack.h
│   └── parse-args.h
├── src/               # Source files
│   ├── main.c
│   ├── command_help.c
│   ├── command-saver.c
│   ├── list-items.c
│   ├── pack.c
│   └── parse-args.c
├── test/              # Tests
├── docker/            # Dockerfiles for lab and test
├── debian/            # Debian packaging
├── Makefile
└── README.md
```

## License

[MIT](LICENSE)
