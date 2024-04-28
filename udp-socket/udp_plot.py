import matplotlib.pyplot as plt

# Define lists to store RTT values for different TTLs
rtt_values_ttl_2 = []
rtt_values_ttl_8 = []
rtt_values_ttl_16 = []

# File path
file_path = "E:\\SAVED CODES\\6TH SEM CODE\\NETWORK LAB\\UDP SOCKET\\udp_output.txt"

# Read data from the file
with open(file_path, 'r') as file:
    # Read data from the file
    for line in file:
        # Parse each line to extract TTL, P, and Cumulative RTT
        parts = line.strip().split(', ')
        
        # Check if the line contains the expected number of elements
        if len(parts) >= 2:
            ttl = int(parts[0].split('=')[1])
            p_rtt = parts[1].split('=')[1].split(': ')
            p = int(p_rtt[0])
            rtt = float(p_rtt[1])

            # Store Cumulative RTT values in corresponding lists
            if ttl == 2:
                rtt_values_ttl_2.append((p, rtt))
            elif ttl == 8:
                rtt_values_ttl_8.append((p, rtt))
            elif ttl == 16:
                rtt_values_ttl_16.append((p, rtt))

# Extract P and RTT values for plotting
p_values_2, rtt_values_2 = zip(*rtt_values_ttl_2)
p_values_8, rtt_values_8 = zip(*rtt_values_ttl_8)
p_values_16, rtt_values_16 = zip(*rtt_values_ttl_16)

# Plotting
plt.figure(figsize=(10, 6))

# Plot for TTL=2
plt.plot(p_values_2, rtt_values_2, label="TTL=2",marker='o', linestyle='-', linewidth=2)

# Plot for TTL=8
plt.plot(p_values_8, rtt_values_8, label="TTL=8",marker='o', linestyle='-', linewidth=2)

# Plot for TTL=16
plt.plot(p_values_16, rtt_values_16, label="TTL=16",marker='o', linestyle='-', linewidth=2)

# Add labels and title
plt.xlabel("Payload Length (P)")
plt.ylabel("Cumulative RTT in ms (for 50 datagrams in total)")
plt.title("Cumulative RTT vs. Payload Length for Different TTLs (2021CSB071_prayas)")
plt.grid(True)
plt.legend()



# Get all unique P values
all_p_values = sorted(set(p_values_2 + p_values_8 + p_values_16))

# Set custom ticks for x-axis (P values)
plt.xticks(all_p_values, all_p_values)



# Set the range of y-axis (Time) from 0 to 5000
plt.ylim(900, 2000)

plt.xlim(0,1100)


# # Extract P values from the data
# payload_values = [int(line.split('=')[2].split(':')[0]) for line in lines]
# # Set custom ticks for x-axis (P values)
# plt.xticks(payload_values, payload_values)

# Show plot
plt.show()
