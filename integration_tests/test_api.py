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
    command_args = ["0.0.0.0", "5000", "3"]

    process = subprocess.Popen([binary_path] + command_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    try:
        assert wait_for_server(healthcheck_url), "Server failed to start"

        post_url = "http://localhost:5000/paths/eventname"
        payload = {
            "values": [1, 2, 3],
            "date": 1738331323010
        }
        response = requests.post(post_url, json=payload)
        assert response.status_code == 200, f"Unexpected status code: {response.status_code}"
        payload = {
            "values": [3, 4, 5],
            "date": 1738331323015
        }
        response = requests.post(post_url, json=payload)
        assert response.status_code == 200, f"Unexpected status code: {response.status_code}"

        get_url = "http://localhost:5000/paths/eventname/meanLength"
        response = requests.get(get_url)
        assert response.status_code == 200, f"Unexpected status code: {response.status_code}"
        assert response.json().mean == 3.0, f"Unexpected response: {response.json()}"

    finally:
        process.terminate()
        try:
            process.wait(timeout=10)
        except:
            process.kill()
        stdout, stderr = process.communicate()
        print("stdout:", stdout)
        print("stderr:", stderr)

if __name__ == "__main__":
    pytest.main(["-v", "test_api.py"])