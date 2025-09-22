# Git Invoice Generator

This Python script automatically generates HTML invoices from git commit history.

## Features

- Parses git commits for a specific date
- Calculates time duration between commits
- Applies 0.5 hour minimum for first and last commits (no start/end time)
- Uses actual commit times for intermediate commits
- Generates professional HTML invoice with detailed line items
- Customizable hourly rate and invoice numbering

## Usage

### Basic Usage
```bash
python3 generate_invoice.py --date 2025-07-19 --rate 25.00 --invoice-num 0009
```

### Advanced Options
```bash
python3 generate_invoice.py \
  --date 2025-07-19 \
  --rate 25.00 \
  --invoice-num 0009 \
  --author "Your Name" \
  --output my_invoice.html \
  --week-ending 2025-07-19
```

## Command Line Arguments

- `--date` (required): Date to generate invoice for (YYYY-MM-DD format)
- `--rate` (optional): Hourly rate in dollars (default: 25.00)
- `--invoice-num` (optional): Invoice number (default: 0009)
- `--author` (optional): Git author name (default: current git user)
- `--output` (optional): Output HTML file name (default: generated_invoice.html)
- `--week-ending` (optional): Week ending date for invoice header (default: same as date)

## Examples

### Generate invoice for today with $30/hour rate
```bash
python3 generate_invoice.py --date $(date +%Y-%m-%d) --rate 30.00 --invoice-num 0010
```

### Generate invoice for specific author
```bash
python3 generate_invoice.py --date 2025-07-19 --author "John Doe" --rate 25.00
```

### Generate invoice with custom output file
```bash
python3 generate_invoice.py --date 2025-07-19 --output "invoice_july_19.html"
```

## How It Works

1. **Git Parsing**: Uses `git log` to retrieve commits for the specified date and author
2. **Time Calculation**: 
   - First commit: 0.5 hours (no start time available)
   - Middle commits: Actual time between consecutive commits
   - Last commit: 0.5 hours (no end time available)
3. **HTML Generation**: Creates detailed invoice with:
   - Commit timestamp
   - Commit hash (short)
   - Commit message description
   - Duration in hours
   - Line item total
4. **Template Usage**: Attempts to use existing `example_invoice.html` as template, falls back to built-in template

## Requirements

- Python 3.6+
- Git repository with commit history
- Existing `style.css` file for styling (optional)

## Output

The script generates an HTML file with:
- Professional invoice header with client/billing information
- Detailed table of git commits with individual line items
- Time tracking and cost calculations
- Total hours and amount

## Notes

- The script automatically detects the current git user if no author is specified
- Very short commits (1-2 minutes) are billed at actual time, not rounded up
- Only first and last commits get the 0.5 hour minimum
- CSS styling is preserved from existing template files