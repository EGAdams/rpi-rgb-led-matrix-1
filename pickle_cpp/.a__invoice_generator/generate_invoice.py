#!/usr/bin/env python3
"""
Git Invoice Generator

This script generates an HTML invoice from git commits for a specific date range.
It calculates time between commits and generates detailed line items.

Usage:
    python generate_invoice.py --date 2025-07-19 --rate 25.00 --invoice-num 0009
"""

import argparse
import subprocess
import re
from datetime import datetime, timedelta
from typing import List, Dict, Tuple
import os

class GitCommit:
    def __init__(self, hash: str, timestamp: str, message: str):
        self.hash = hash
        self.timestamp = datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S %z")
        self.message = message
        self.duration_hours = 0.0
        self.amount = 0.0

class InvoiceGenerator:
    def __init__(self, hourly_rate: float = 25.0):
        self.hourly_rate = hourly_rate
        self.commits: List[GitCommit] = []
        
    def get_git_commits(self, date: str, author: str = None) -> List[GitCommit]:
        """Get git commits for a specific date"""
        if author is None:
            # Get current git user
            try:
                author = subprocess.check_output(['git', 'config', 'user.name'], 
                                               text=True).strip()
            except subprocess.CalledProcessError:
                author = ".*"  # Match any author if can't get current user
        
        # Git log command for the specific date
        cmd = [
            'git', 'log',
            f'--since={date} 00:00:00',
            f'--until={date} 23:59:59',
            f'--author={author}',
            '--pretty=format:%h|%ci|%s',
            '--reverse'  # Chronological order
        ]
        
        try:
            output = subprocess.check_output(cmd, text=True).strip()
            if not output:
                print(f"No commits found for {date} by {author}")
                return []
                
            commits = []
            for line in output.split('\n'):
                if line.strip():
                    parts = line.split('|', 2)
                    if len(parts) == 3:
                        hash_short, timestamp, message = parts
                        commits.append(GitCommit(hash_short, timestamp, message))
            
            return commits
        except subprocess.CalledProcessError as e:
            print(f"Error getting git commits: {e}")
            return []
    
    def calculate_durations(self, commits: List[GitCommit]) -> List[GitCommit]:
        """Calculate time durations between commits"""
        if not commits:
            return []
        
        # First commit gets 0.5 hours (no start time)
        commits[0].duration_hours = 0.5
        commits[0].amount = round(commits[0].duration_hours * self.hourly_rate, 2)
        
        # Calculate durations between commits
        for i in range(1, len(commits)):
            duration = commits[i].timestamp - commits[i-1].timestamp
            duration_hours = duration.total_seconds() / 3600
            commits[i].duration_hours = round(duration_hours, 2)
            commits[i].amount = round(commits[i].duration_hours * self.hourly_rate, 2)
        
        # Last commit gets 0.5 hours (no end time)
        if len(commits) > 1:
            commits[-1].duration_hours = 0.5
            commits[-1].amount = round(commits[-1].duration_hours * self.hourly_rate, 2)
        
        return commits
    
    def generate_html_rows(self, commits: List[GitCommit]) -> str:
        """Generate HTML table rows for commits"""
        rows = []
        for commit in commits:
            time_str = commit.timestamp.strftime("%H:%M")
            rows.append(f'''      <tr>
        <td class="datestring">{time_str}</td>
        <td>{commit.hash}</td>
        <td class="commit-description">{commit.message}</td>
        <td>{commit.duration_hours}</td>
        <td>${self.hourly_rate:.2f}</td>
        <td>${commit.amount:.2f}</td>
      </tr>''')
        return '\n'.join(rows)
    
    def calculate_total(self, commits: List[GitCommit]) -> Tuple[float, float]:
        """Calculate total hours and amount"""
        total_hours = sum(commit.duration_hours for commit in commits)
        total_amount = sum(commit.amount for commit in commits)
        return total_hours, total_amount
    
    def generate_invoice_html(self, commits: List[GitCommit], date: str, 
                            invoice_num: str, week_ending: str = None) -> str:
        """Generate complete HTML invoice"""
        if week_ending is None:
            week_ending = date
            
        total_hours, total_amount = self.calculate_total(commits)
        
        # Read the template (or create a basic one)
        template = self.get_html_template()
        
        # Generate commit rows
        commit_rows = self.generate_html_rows(commits)
        
        # Replace placeholders
        html = template.replace('{{INVOICE_NUM}}', invoice_num)
        html = html.replace('{{WEEK_ENDING}}', week_ending)
        html = html.replace('{{COMMIT_ROWS}}', commit_rows)
        html = html.replace('{{TOTAL_AMOUNT}}', f'${total_amount:.2f}')
        
        return html
    
    def get_html_template(self) -> str:
        """Get HTML template - try to read existing file or use default"""
        template_path = 'example_invoice.html'
        if os.path.exists(template_path):
            try:
                with open(template_path, 'r') as f:
                    content = f.read()
                
                # Extract the structure and replace the table body
                # Find the tbody section and replace with placeholder
                start_marker = '<!-- July 19, 2025 Work -->'
                end_marker = '<tr>\n        <td colspan="5"'
                
                start_idx = content.find(start_marker)
                end_idx = content.find(end_marker)
                
                if start_idx != -1 and end_idx != -1:
                    before = content[:start_idx]
                    after = content[end_idx:]
                    template = before + '<!-- Generated Commits -->\n{{COMMIT_ROWS}}\n  \n      ' + after
                    
                    # Replace other placeholders
                    template = re.sub(r'Invoice #:</strong> \d+', 'Invoice #:</strong> {{INVOICE_NUM}}', template)
                    template = re.sub(r'Week Ending:</strong> [\d\-/]+', 'Week Ending:</strong> {{WEEK_ENDING}}', template)
                    template = re.sub(r'<strong>\$[\d.]+</strong>', '<strong>{{TOTAL_AMOUNT}}</strong>', template)
                    
                    return template
            except Exception as e:
                print(f"Error reading template: {e}")
        
        # Fallback basic template
        return self.get_basic_template()
    
    def get_basic_template(self) -> str:
        """Basic HTML template if no existing file found"""
        return '''<html>
<head>
  <title>Invoice</title>
  <link rel="stylesheet" href="style.css">
</head>
<body>
  <div class="invoice-header">
    <h1>Invoice</h1>
    <div>
      <p><strong>Invoice #:</strong> {{INVOICE_NUM}}</p>
      <p><strong>Week Ending:</strong> {{WEEK_ENDING}}</p>
    </div>
  </div>
  
  <div class="addresses" style="display: flex; gap: 8ch;">
    <div>
      <p><strong>BILL TO:</strong></p>
      <p>
        Louis H Adams Jr.<br>
        836 41st Ave N<br>
        Saint Petersburg, FL 33703<br>
        (813)368-5960<br>
        eg@americansjewelry.com
      </p>
    </div>
    <div>
      <p><strong>CLIENT:</strong></p>
      <p>
        Roy L. McGreevy<br>
        ScorePro Sports Inc.<br>
        11860 Lake Allen Dr.<br>
        Largo, FL 33773<br>
        (727)466-7975<br>
        kscbtv@gmail.com
      </p>
    </div>
  </div>
  
  <table>
    <thead class="table-header" style="background-color: #a8a1a199;">
      <tr>
        <th>Time</th>
        <th>Commit ID</th>
        <th>Description</th>
        <th>Hours</th>
        <th>Unit Price</th>
        <th>Total</th>
      </tr>
    </thead>
    <tbody>
{{COMMIT_ROWS}}
  
      <tr>
        <td colspan="5" style="text-align: right;"><strong>TOTAL:</strong></td>
        <td><strong>{{TOTAL_AMOUNT}}</strong></td>
      </tr>
    </tbody>
  </table>
</body>
</html>'''

def main():
    parser = argparse.ArgumentParser(description='Generate invoice from git commits')
    parser.add_argument('--date', required=True, help='Date to generate invoice for (YYYY-MM-DD)')
    parser.add_argument('--rate', type=float, default=25.0, help='Hourly rate (default: 25.0)')
    parser.add_argument('--invoice-num', default='0009', help='Invoice number (default: 0009)')
    parser.add_argument('--author', help='Git author name (default: current git user)')
    parser.add_argument('--output', default='generated_invoice.html', help='Output file name')
    parser.add_argument('--week-ending', help='Week ending date (default: same as date)')
    
    args = parser.parse_args()
    
    # Validate date format
    try:
        datetime.strptime(args.date, '%Y-%m-%d')
    except ValueError:
        print("Error: Date must be in YYYY-MM-DD format")
        return 1
    
    # Generate invoice
    generator = InvoiceGenerator(args.rate)
    
    print(f"Getting git commits for {args.date}...")
    commits = generator.get_git_commits(args.date, args.author)
    
    if not commits:
        print("No commits found for the specified date")
        return 1
    
    print(f"Found {len(commits)} commits")
    
    # Calculate durations
    commits = generator.calculate_durations(commits)
    
    # Generate HTML
    week_ending = args.week_ending or args.date
    html = generator.generate_invoice_html(commits, args.date, args.invoice_num, week_ending)
    
    # Write output
    with open(args.output, 'w') as f:
        f.write(html)
    
    # Calculate totals for summary
    total_hours, total_amount = generator.calculate_total(commits)
    
    print(f"\nInvoice generated: {args.output}")
    print(f"Total commits: {len(commits)}")
    print(f"Total hours: {total_hours:.2f}")
    print(f"Total amount: ${total_amount:.2f}")
    
    return 0

if __name__ == '__main__':
    exit(main())