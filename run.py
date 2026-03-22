import subprocess
import os
import webbrowser
import time
import sys

def run_project():
    root_dir = os.path.dirname(os.path.abspath(__file__))
    backend_dir = os.path.join(root_dir, 'backend')
    frontend_path = os.path.join(root_dir, 'frontend', 'index.html')

    print("🚀 Starting Library Management System...")

    # Start backend
    print("📡 Starting Backend (Flask)...")
    try:
        # Check if backend/app.py exists
        if not os.path.exists(os.path.join(backend_dir, 'app.py')):
            print(f"❌ Error: Could not find backend/app.py in {backend_dir}")
            return

        # Start the Flask process
        backend_process = subprocess.Popen(
            [sys.executable, 'app.py'],
            cwd=backend_dir,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT
        )
        
        # Wait a moment for it to start
        time.sleep(2)
        
        # Check if it crashed immediately
        if backend_process.poll() is not None:
            print("❌ Backend failed to start. Check your dependencies.")
            return

        print("✅ Backend is running!")
        
        # Open frontend
        print(f"🌐 Opening Frontend: {frontend_path}")
        webbrowser.open(f'file:///{frontend_path}')
        
        print("\n--- Application is live! ---")
        print("Keep this terminal open to keep the backend running.")
        print("Press Ctrl+C to stop.")
        
        # Keep it alive
        backend_process.wait()

    except KeyboardInterrupt:
        print("\n👋 Stopping Library Management System...")
        backend_process.terminate()
    except Exception as e:
        print(f"❌ An error occurred: {e}")

if __name__ == "__main__":
    run_project()
