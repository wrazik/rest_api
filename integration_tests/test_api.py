import subprocess
import time
import requests
import pytest

def wait_for_server(url, timeout=30):
    start_time = time.time()
    while time.time() - start_time < timeout:
        try:
            response = requests.get(url)
            if response.status_code == 200:
                return True
        except requests.ConnectionError:
            time.sleep(1)
    return False

def test_rest_api():
    binary_path = "../builds/ninja-multi-vcpkg/Release/rest_api"
    healthcheck_url = "http://localhost:5000/healthcheck"
    post_url = "http://localhost:5000/your_endpoint"
    payload = {"key": "value"}
    command_args = ["0.0.0.0", "5000", "3"]

    process = subprocess.Popen([binary_path] + command_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    try:
        assert wait_for_server(healthcheck_url), "Server failed to start"

        response = requests.post(post_url, json=payload)
        assert response.status_code == 200, f"Unexpected status code: {response.status_code}"
        assert "expected_key" in response.json(), "Response does not contain expected data"

    finally:
        process.terminate()
        process.wait()
        assert process.returncode is not None, "Process did not terminate properly"

if __name__ == "__main__":
    pytest.main(["-v", "test_api.py"])