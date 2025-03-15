import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

class AboutScreen extends StatelessWidget {
  const AboutScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return Scaffold(
      appBar: AppBar(
        title: const Text('About'),
        backgroundColor: const Color(0xFF1A1A1A),
        elevation: 0,
      ),
      backgroundColor: const Color(0xFF121212),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Center(
              child: Image.asset(
                'assets/images/energy_monitor.png',
                height: 120,
              ),
            ),
            const SizedBox(height: 24),
            Text(
              'Energy Monitor',
              style: theme.textTheme.headlineSmall?.copyWith(
                color: theme.colorScheme.primary,
                fontWeight: FontWeight.bold,
              ),
            ),
            const SizedBox(height: 8),
            Text(
              'Version 1.0.0',
              style: theme.textTheme.bodyMedium?.copyWith(
                color: theme.colorScheme.secondary.withAlpha(179),
              ),
            ),
            _buildSection(
              theme,
              'Description',
              'A real-time energy monitoring system that provides detailed metrics about power consumption and quality. Built using ESP32 and PZEM-004T.',
            ),
            _buildSection(
              theme,
              'Key Features',
              '• Real-time power monitoring\n'
                  '• Battery status tracking\n'
                  '• Power quality analysis\n'
                  '• Energy consumption tracking\n'
                  '• Advanced electrical metrics',
            ),
            _buildSection(
              theme,
              'System Components',
              '• ESP32 microcontroller for data processing\n'
                  '• PZEM-004T energy meter modules\n'
                  '• 16A and 30A input support\n'
                  '• Battery: 3.6V 2600mAh Li-ion (18650)\n'
                  '• Charging module with boost converter\n'
                  '• Level shifters for safe communication',
            ),
            _buildSection(
              theme,
              'Mobile Application',
              '• Built with Flutter & Firebase\n'
                  '• Real-time monitoring\n'
                  '• Data analysis capabilities\n'
                  '• Remote access functionality\n'
                  '• Secure authentication',
            ),
            _buildDeveloperInfo(context),
            const SizedBox(height: 24),
            Center(
              child: Text(
                '© ${DateTime.now().year} Energy Monitor',
                style: theme.textTheme.bodySmall?.copyWith(
                  color: theme.colorScheme.secondary.withAlpha(179),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildSection(ThemeData theme, String title, String content) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 24),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            title,
            style: theme.textTheme.titleMedium?.copyWith(
              color: theme.colorScheme.primary,
              fontWeight: FontWeight.bold,
            ),
          ),
          const SizedBox(height: 8),
          Text(
            content,
            style: theme.textTheme.bodyMedium?.copyWith(
              color: theme.colorScheme.secondary,
              height: 1.5,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildDeveloperInfo(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: const Color(0xFF1A1A1A),
        borderRadius: BorderRadius.circular(12),
        border: Border.all(color: const Color(0xFF5DB996).withAlpha(77)),
      ),
      child: Column(
        children: [
          Row(
            children: [
              const CircleAvatar(
                backgroundColor: Color(0xFF5DB996),
                child: Icon(Icons.code, color: Colors.white),
              ),
              const SizedBox(width: 16),
              Expanded(
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: const [
                    Text(
                      'Developed by',
                      style: TextStyle(color: Colors.white70, fontSize: 14),
                    ),
                    Text(
                      'Abinav Bino',
                      style: TextStyle(
                        color: Color(0xFFE3F0AF),
                        fontSize: 16,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
          const SizedBox(height: 16),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              _buildSocialButton(
                icon: Icons.link,
                label: 'LinkedIn',
                url: 'https://www.linkedin.com/in/abinav-bino/',
                color: const Color(0xFF0A66C2),
              ),
              const SizedBox(width: 16),
              _buildSocialButton(
                icon: Icons.code,
                label: 'GitHub',
                url: 'https://github.com/Iam-Madman',
                color: const Color(0xFF333333),
              ),
            ],
          ),
        ],
      ),
    );
  }

  Widget _buildSocialButton({
    required IconData icon,
    required String label,
    required String url,
    required Color color,
  }) {
    return Builder(
      builder:
          (context) => ElevatedButton.icon(
            onPressed: () => _launchURL(url, context),
            icon: Icon(icon, size: 18),
            label: Text(label),
            style: ElevatedButton.styleFrom(
              backgroundColor: color,
              foregroundColor: Colors.white,
              padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(8),
              ),
            ),
          ),
    );
  }

  Future<void> _launchURL(String url, BuildContext context) async {
    try {
      final Uri uri = Uri.parse(url);
      if (!await launchUrl(uri, mode: LaunchMode.externalApplication)) {
        throw Exception('Could not launch $url');
      }
    } catch (e) {
      debugPrint('Error launching URL: $e');
      if (context.mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Could not open $url. Please try again.'),
            backgroundColor: Colors.red,
            action: SnackBarAction(
              label: 'Retry',
              textColor: Colors.white,
              onPressed: () => _launchURL(url, context),
            ),
          ),
        );
      }
    }
  }
}
