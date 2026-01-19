import argparse
import os
import subprocess
import sys
import time
import xml.etree.ElementTree as ET
from datetime import datetime

def run_command(cmd, shell=True):
    """Run a shell command and return output."""
    try:
        result = subprocess.run(
            cmd,
            shell=shell,
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True
        )
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def parse_gtest_xml(xml_path):
    """Parse GoogleTest XML output and return a summary."""
    if not os.path.exists(xml_path):
        return {"total": 0, "passed": 0, "failed": 0, "tests": []}

    tree = ET.parse(xml_path)
    root = tree.getroot()
    
    summary = {
        "tests": [],
        "total": int(root.attrib.get("tests", 0)),
        "failures": int(root.attrib.get("failures", 0)),
        "errors": int(root.attrib.get("errors", 0)),
        "time": root.attrib.get("time", "0")
    }
    
    for testcase in root.findall(".//testcase"):
        test_info = {
            "name": testcase.attrib.get("name"),
            "classname": testcase.attrib.get("classname"),
            "status": "PASS",
            "message": ""
        }
        if testcase.find("failure") is not None:
            test_info["status"] = "FAIL"
            test_info["message"] = testcase.find("failure").text
        summary["tests"].append(test_info)
        
    return summary

def generate_html_report(summary, logs, output_file):
    """Generate a simple HTML report."""
    html = f"""
    <html>
    <head>
        <title>Platform Test Report</title>
        <style>
            body {{ font-family: sans-serif; margin: 20px; }}
            .pass {{ color: green; }}
            .fail {{ color: red; }}
            .summary {{ background: #f0f0f0; padding: 10px; border-radius: 5px; }}
            table {{ border-collapse: collapse; width: 100%; }}
            th, td {{ border: 1px solid #ddd; padding: 8px; text-align: left; }}
            th {{ background-color: #4CAF50; color: white; }}
        </style>
    </head>
    <body>
        <h1>Platform Test Report</h1>
        <div class="summary">
            <h3>Summary</h3>
            <p>Date: {datetime.now()}</p>
            <p>Total Tests: {summary['total']}</p>
            <p>Failures: {summary['failures'] + summary['errors']}</p>
            <p>Time: {summary['time']}s</p>
        </div>
        
        <h3>Test Details</h3>
        <table>
            <tr><th>Class</th><th>Test</th><th>Status</th><th>Message</th></tr>
    """
    
    for test in summary['tests']:
        status_class = "pass" if test['status'] == "PASS" else "fail"
        html += f"""
            <tr>
                <td>{test['classname']}</td>
                <td>{test['name']}</td>
                <td class="{status_class}">{test['status']}</td>
                <td><pre>{test['message'][:200]}</pre></td>
            </tr>
        """
        
    html += """
        </table>
        
        <h3>System Logs (Tail)</h3>
        <pre>
    """
    html += logs[-2000:] if len(logs) > 0 else "No logs available."
    html += """
        </pre>
    </body>
    </html>
    """
    
    with open(output_file, 'w') as f:
        f.write(html)

def main():
    parser = argparse.ArgumentParser(description="Run Platform Tests")
    parser.add_argument("--bin", default="./build/pcie_tests", help="Path to test binary")
    parser.add_argument("--out", default="test_results", help="Output directory")
    args = parser.parse_args()
    
    if not os.path.exists(args.out):
        os.makedirs(args.out)
        
    xml_output = os.path.join(args.out, "test_detail.xml")
    
    print(f"Running tests from {args.bin}...")
    
    # Run Generic Platform Tests
    cmd = f"{args.bin} --gtest_output=xml:{xml_output}"
    # Use sudo if strictly required, but usually we run the script with sudo
    ret, stdout, stderr = run_command(cmd)
    
    print("Test execution finished.")
    if ret != 0:
        print(f"Tests failed with exit code {ret}")
        print("Stdout:", stdout)
        print("Stderr:", stderr)
    
    # Capture dmesg
    print("Capturing dmesg...")
    _, dmesg, _ = run_command("dmesg | tail -n 100")
    
    # Process Results
    summary = parse_gtest_xml(xml_output)
    
    # Generate Reports
    html_report = os.path.join(args.out, "report.html")
    generate_html_report(summary, dmesg, html_report)
    
    print(f"Report generated: {html_report}")
    
    # Print Console Summary
    print("\n=== Test Summary ===")
    print(f"Total: {summary['total']}")
    print(f"Failed: {summary['failures'] + summary['errors']}")
    
    return ret

if __name__ == "__main__":
    sys.exit(main())
