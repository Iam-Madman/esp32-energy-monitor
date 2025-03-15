class MetricDescriptions {
  static const Map<String, String> descriptions = {
    'Voltage':
        'The potential difference between two points in an electrical circuit, determining the force that drives current. It is measured in volts (V).',
    'Current':
        'The rate of flow of electric charge in a circuit, measured in amperes (A). Higher current indicates more electrons flowing through a conductor.',
    'Power':
        'The real (active) power consumed by a load, measured in watts (W). It represents the actual energy used to perform work (e.g., lighting a bulb or running a motor).',
    'Energy':
        'The total amount of electrical power consumed over time, measured in watt-hours (Wh) or kilowatt-hours (kWh). It indicates electricity usage, often used for billing.',
    'Frequency':
        'The number of complete AC voltage cycles per second, measured in hertz (Hz). Standard values are 50 Hz (Europe, India) and 60 Hz (USA).',
    'Apparent Power':
        'The combination of real and reactive power, measured in volt-amperes (VA). It represents the total power supplied by the source.\n\nCalculation: S = V × I\nWhere:\nS = Apparent Power (VA)\nV = Voltage (V)\nI = Current (A)',
    'Power Factor':
        'A dimensionless ratio (0 to 1) that indicates how efficiently electrical power is converted into useful work. A power factor close to 1 means minimal energy loss.',
    'Reactive Power':
        'The power stored and released due to inductive or capacitive loads, measured in volt-amperes reactive (VAR).\n\nCalculation: Q = S × √(1 - PF²)\nWhere:\nQ = Reactive Power (VAR)\nS = Apparent Power (VA)\nPF = Power Factor',
    'Load Impedance':
        'The total opposition (resistance + reactance) to AC current flow, measured in ohms (Ω).\n\nCalculation: Z = V ÷ I\nWhere:\nZ = Load Impedance (Ω)\nV = Voltage (V)\nI = Current (A)',
    'THD':
        'Total Harmonic Distortion - A measure of waveform distortion caused by harmonics.\n\nCalculation: THD = (D ÷ P) × 100%\nWhere:\nD = Distortion Power\nP = Fundamental Power',
    'Distortion Power':
        'The power lost due to harmonic distortion in a circuit.\n\nCalculation: D = √(S² - P² - Q²)\nWhere:\nD = Distortion Power (VA)\nS = Apparent Power (VA)\nP = Active Power (W)\nQ = Reactive Power (VAR)',
    'Power Quality':
        'A metric that assesses the overall quality of power supply.\n\nCalculation: PQ = PF × (1 - THD/100)\nWhere:\nPQ = Power Quality (0-1)\nPF = Power Factor\nTHD = Total Harmonic Distortion (%)',
  };
}
